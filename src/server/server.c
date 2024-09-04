#ifdef linux
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h> 
#include <netinet/in.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif
// For ..
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "users.h"
#include "protocol.h"
#include "signal.h"

// Signal handler function
void signal_handler(int signal_num);

int main(int argc, char *argv[]) {
    // Register signal handlers
    signal(SIGINT, signal_handler);  // Handle Ctrl+C interrupt
    signal(SIGSEGV, signal_handler); // Handle segmentation fault
    signal(SIGABRT, signal_handler); // Handle abort
    initializeUser();

    #ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock.\n");
        exit(EXIT_FAILURE);
    }
    #endif

    // input: Port
	if(argc != 2) {
		printf("Please input port number!\n");
		return 0;
	}

    // Create Users and Gamesc
    int port_number = atoi(argv[1]);
    struct sockaddr_in servaddr, clieaddr;
    
    // Tao server socket
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Failed to create socket!");
        exit(0);
    }
    // Set reuse option
    int tempfd = 1;
    int check = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&tempfd, sizeof(int));
    if (check == -1)
        printf("Failed to set re-use address!");

    //Step 2: Bind address to socket
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port_number);

    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) {
        printf("Failed to set-up server!");
        exit(0);
    }
    
    // Step 3: listen to sockfd
    check = listen(sockfd, 10);
    if (check == -1) {
        printf("Failed to set-up server!");
        exit(1);
    }
    // Mỗi client kết nối đến server, nếu được chấp nhận thì server sẽ tạo ra một socket để giao tiếp với client đó đến khi 1 trong 2 bên đóng.
    // Ta đã biết đến forking server ( mỗi khi chấp nhận một connect thì tạo 1 process để xử lý nó ), hoặc interactive server ( liên tục kiểm
    // tra xem có kết nối không.
    // Ở đây ta dùng select(), nó sẽ nhìn (watch) trên nhiều file desciptor, socket cũng là 1 loại file descriptor) để xem có sự kiện nào xảy ra
    // không?
    // Có 3 loại sự kiện trên 1 file desciptor mà ta có xem xét là : ready to read, ready to write, exception.
    // Ở đây ta chỉ quan tâm đến ready to read, sự kiện có 1 kết nối đến server coi như là ột /sự kiện ready to read trên server Socket
    // ta cần có 1 tập chứa các socket
    int len = sizeof(struct sockaddr_in);
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    fd_set masterfds; // tập readfds để check các socket, 1 tập để lưu lại nhưng thay đổi của tập readfds.
    fd_set readfds;
    FD_ZERO(&masterfds);
    FD_ZERO(&readfds);
    FD_SET(sockfd, &masterfds); // add serverSock vào tập masterfds.

    int max_fd = sockfd;
    int n_select;
    do {
        memcpy(&readfds, &masterfds, sizeof(masterfds)); // Copy masterfds vao readfds để đợi sự kiện
        n_select = select(max_fd + 1, &readfds, NULL, NULL, &timeout);
        // Hàm này sẽ block chương trình đến khi có 1 sự kiên ready to read xảy ra
        if (n_select < 0) {
            printf("Server failed to select!");
            exit(0);
        }
        else if (n_select == 0) {
            // No connect/message
        }
        else {
            for (tempfd = 0; tempfd <= max_fd; tempfd++) {// Duyệt tất cả các socket đến max_fd.
                if (FD_ISSET(tempfd, &readfds)) {// Nếu serverSock có sự kiện ready to read nghĩa là nó có kết nối mới.
                    int close_fd = 0; // Kiểm tra xem socket có nên được đóng sau khi xử lý không
                    if (tempfd == sockfd) {
                        int newCon = accept(sockfd, (struct sockaddr *)&clieaddr, &len); // Chấp nhận kết nối đó
                        printf("New connection \n");

                        FD_SET(newCon, &masterfds); // Thêm vào masterfds set để check sự kiện.
                        if (newCon > max_fd)
                            max_fd = newCon;
                    }
                    else {
                        Request *req = createRequest();
                        Response *res = createResponse();
                        
                        printf("Receive data in socket %d\n", tempfd);
                        int nrecv = recvReq(tempfd, req, sizeof(Request), 0);
                        if (nrecv == -1) {
                            printf("Error on socket %d!\nClosing clientfd!\n", tempfd);
                            close_fd = 1;
                        }
                        else {
                            switch (req->code) {
                                case CLOSE:
                                // Close command promp/terminal
                                break;
                                case SIGN_IN:
                                handleSignin(tempfd, req, res);
                                break;
                                case SIGN_UP:
                                handleSignup(tempfd, req, res);
                                break;
                                case SIGN_OUT:
                                handleSignout(tempfd, req, res);
                                break;
                                case FIND_GAME:
                                handleFindGame(tempfd, req, res);
                                break;
                                case PICK:
                                handlePick(tempfd, req, res);
                                break;
                                case REDO_ASK:
                                handleRedoAsk(tempfd, req, res);
                                break;
                                case REDO_AGREE:
                                handleRedoAgree(tempfd, req, res);
                                break;
                                case QUIT:
                                handleQuit(tempfd, req, res);
                                break;
                                case GET_USERNAME_REPLAYS:
                                handleShowReplay(tempfd, req, res);
                                break;
                                case REPLAY_CONTROL:
                                handleControlReplay(tempfd, req, res);
                                break;
                                case GET_USERS:
                                handleGetUsers(tempfd, req, res);
                                break;
                                case ADD_USER:
                                handleAdminAddUser(tempfd, req, res);
                                break;
                                case EDIT_USER:
                                handleAdminEditUser(tempfd, req, res);
                                break;
                                case DELETE_USER:
                                handleAdminDeleteUser(tempfd, req, res);
                                break;
                                case DELETE_GAME:
                                //
                                break;
                                case GET_REPLAYID_MOVES:
                                handleWatchReplay(tempfd, req, res);
                                break;
                                default:
                                break;
                            }
                        }
                        free(req);
                        free(res);
                    }
                    if (close_fd == 1) {
                        FD_CLR(tempfd, &masterfds);
                        #ifdef linux
                        close(tempfd);
                        #endif
                        #ifdef _WIN32
                        closesocket(tempfd);
                        #endif
                    }
                }
            }
        }
    } while (1);

    #ifdef _WIN32
    WSACleanup();
    #endif
    return 0;
}

// Signal handler function
void signal_handler(int signal_num) {
    logoutUsers();
    writeUsersIni();
    printf("Caught signal %d\n", signal_num);
    if (signal_num == SIGSEGV) {
        printf("Segmentation fault encountered!\n");
    } else if (signal_num == SIGINT) {
        printf("Interrupt signal caught. Exiting gracefully...\n");
        exit(0);
    } else if (signal_num == SIGABRT) {
        printf("Abnormal termination triggered!\n");
        exit(1);
    }
}