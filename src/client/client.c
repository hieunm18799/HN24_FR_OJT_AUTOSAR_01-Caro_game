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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "protocol.h"
#include "top_screen.h"

void handle_sigint(int sig);
void startGUI(int sockfd);
DWORD WINAPI ReceiveHandler(void *socket_desc);

int sockfd;

int main(int argc, char const *argv[]) {
    #ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock.\n");
        exit(EXIT_FAILURE);
    }
    #endif
    signal(SIGINT, handle_sigint);
    // input: IPv4 + Port
	if(argc != 3) {
		printf("Please input IP address and port number\n");
		return 0;
	}

    struct sockaddr_in servaddr;
    // Create TCP socket
    sockfd = socket(PF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        printf("Failed to create socket.\n");
        exit(0);
    }
    // Specify server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    int check = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    #ifdef _WIN32
    if (check == SOCKET_ERROR) {
        printf("Connection failed. Error: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 0;
    }
    #endif
    #ifdef linux
    if (check == -1) {
        printf("Failed to connect to server\n");
        exit(0);
    }
    #endif
    HANDLE recvThread = CreateThread(NULL, 0, ReceiveHandler, (void *)&sockfd, 0, NULL);
    if (recvThread == NULL) {
        printf("Could not create receive thread. Error: %d\n", GetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    startGUI(sockfd);

    #ifdef _WIN32
    WSACleanup();
    #endif
    close(sockfd);
    return 0;
}

void handle_sigint(int sig) {
    printf("\nCaught signal %d (SIGINT), exiting...\n", sig);
    exit(0);
}

void startGUI(int sockfd) {
    drawInitialUI();
    struct timeval timeout;
    int result;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;  // 0.1 seconds

    while (1) {
        handleMouseClick(); // Gọi hàm để xử lý sự kiện chuột

        if (Click_flag) { // Nếu có sự kiện click
            Click_flag = 0; // Reset cờ click

            if (currentScreen == VIEW_TOP_NOT_SIGN_IN) { // Nếu đang ở màn hình ban đầu
                handleClickOnInitialScreen();
            } else if (currentScreen == VIEW_SIGN_IN) { // Nếu đang ở màn hình đăng nhập
                handleClickOnSigninScreen();
            } else if (currentScreen == VIEW_SIGN_UP) { // Nếu đang ở màn hình đăng ký
                handleClickOnSignupScreen();
            } else if (currentScreen == VIEW_TOP_SIGNED_IN_ADMIN) {
                openAdmin();
            } else if (currentScreen == VIEW_TOP_SIGNED_IN_USER) {
                openUser();
            } else {

            }
            
        }
    }
}

DWORD WINAPI ReceiveHandler(void *socket_desc) {
    SOCKET sock = *(SOCKET *)socket_desc;

    while (1) {
        Response *res = (Response *)malloc(sizeof(Response));
        int rcvBytes = recvRes(sockfd, res, sizeof(Response), 0);
        if (rcvBytes != -1) {
            switch (res->code) {
            case SIGN_IN_SUCCESS:
                readSigninSuccess(res->data, signed_in_username, signed_in_role);
                dashbroad();
                break;
            case USERNAME_NOT_EXISTED:
                // Show error
                break;
            case WRONG_PASSWORD:
                // Show error
                break;
            case ACCOUNT_BUSY:
                // Show error
                break;
            case SIGN_UP_SUCCESS:
                drawSignInUI(); // Mở giao diện đăng nhập sau khi đăng ký
                break;
            case USERNAME_EXISTED:
                // Show error
                break;
            case SIGN_UP_INPUT_WRONG:
                // Show error
                break;
            default:
                break;
            }
        }
        free(res);
    }
    return 0;
}