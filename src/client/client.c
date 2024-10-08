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
DWORD WINAPI ReceiveHandler(LPVOID lpParameter);

static int rcvBytes = 0;
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
    HANDLE recvThread = CreateThread(NULL, 0, ReceiveHandler, NULL, 0, NULL);
    if (recvThread == NULL) {
        printf("Could not create receive thread. Error: %ld\n", GetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    startGUI(sockfd);

    #ifdef _WIN32
    WSACleanup();
    closesocket(sockfd);
    #endif
    #ifdef UNIX
    close(sockfd);
    #endif
    return 0;
}

void handle_sigint(int sig) {
    #ifdef _WIN32
    WSACleanup();
    closesocket(sockfd);
    #endif
    #ifdef UNIX
    close(sockfd);
    #endif
    exit(0);
}

void startGUI(int sockfd) {
    drawInitialUI();

    while (rcvBytes != -1) {
        handleMouseClick(); // Gọi hàm để xử lý sự kiện chuột
        if (currentScreen == VIEW_PLAY_GAME) RedrawPlayCaroBoard();

        if (Click_flag) { // Nếu có sự kiện click
            Click_flag = 0; // Reset cờ click

            switch (currentScreen) {
                case VIEW_TOP_NOT_SIGN_IN:
                    handleClickOnInitialScreen();
                    break;
                case VIEW_SIGN_IN:
                    handleClickOnSigninScreen();
                    break;
                case VIEW_SIGN_UP:
                    handleClickOnSignupScreen();
                    break;
                case VIEW_FIND_GAME:
                    handleClickOnFindPlayerScreen();
                    break;
                case VIEW_PLAY_GAME:
                    MovePlayCaro();
                    break;
                case VIEW_TOP_SIGNED_IN_ADMIN:
                    openAdmin();
                    break;
                case VIEW_TOP_SIGNED_IN_USER:
                    openUser();
                    break;
                case VIEW_ADMIN_MANAGE:
                    openAdminScreen();
                    break;
                case VIEW_ADMIN_USER_MANAGE:
                    handleUserRowClick();
                    handleOnScreenUserManagement();
                    break;
                case VIEW_ADMIN_REPLAY_MANAGE:
                    handleRowClick();
                    handleOnScreenReplayManagement();
                    break;
                case VIEW_REPLAY_LIST:
                    handleRowClick();
                    handleOnScreenReplayInfo();
                    break;
                case VIEW_WATCH_REPLAY:
                    handleClickOnWatchReplayScreen();
                    
                    break;
                default:
                    break;
            }
        }
    }
}



DWORD WINAPI ReceiveHandler(LPVOID lpParameter) {
    while (rcvBytes != -1) {
        Response *res = (Response *)malloc(sizeof(Response));
        rcvBytes = recvRes(sockfd, res, sizeof(Response), 0);
        if (rcvBytes != -1) {
            unsigned char x, y;
            char username[50];
            switch (res->code) {
            case SYNTAX_ERROR:
                break;
            case SIGN_UP_INPUT_WRONG:
                // Show error
                showErrorNotification("Sign-up input is wrong!");
                break;
            case SIGN_IN_INPUT_WRONG:
                // Show error
                showErrorNotification("Sign-up input is wrong!");
                break;
            case USERNAME_NOT_EXISTED:
                // Show error
                showErrorNotification("This account is not registered!");
                break;
            case WRONG_PASSWORD:
                // Show error
                showErrorNotification("Wrong password!");
                break;
            case USERNAME_EXISTED:
                // Show error
                showErrorNotification("Username already exists! Please choose another.");
                break;
            case ACCOUNT_BUSY:
                // Show error
                showErrorNotification("This account is being used by another player!");
                break;
            case SIGN_OUT_FAIL:
                // Show error
                showErrorNotification("Sign-out encountered an error!");
                break;
            case SIGN_IN_SUCCESS:
                readSigninSuccess(res->data, signed_in_username, signed_in_role);
                dashboard();
                break;
            case SIGN_UP_SUCCESS:
                drawSignInUI(); // Mở giao diện đăng nhập sau khi đăng ký
                break;
            case SIGN_OUT_SUCCESS:
                drawInitialUI();
                break;
            case WAITING_PLAYER:
                readWaitingGame(res->data, &game_id);
                drawFindPlayer();
                break;
            case GAME_START:
                readGameStart(res->data, &game_id, &player1_username[0], &player1_win, &player1_lose, &player2_username[0], &player2_win, &player2_lose);
                drawPlayCaroBoard();
                break;
            case YOUR_TURN:
                printMessagePlayCaro(res->message);
                break;
            case OTHER_PLAYER_TURN:
                printMessagePlayCaro(res->message);
                break;
            case PICK_FAIL:
                printMessagePlayCaro(res->message);
                break;
            case PICK_SUCCESS:
                if (readPickSuccess(res->data, username, &x, &y)) addPicked(username, x, y);
                break;
            case REDO_FAIL:
                break;
            case REDO_SUCCESS:
                if (readRedoSuccess(res->data, &x, &y)) redoLastPicked(x, y);
                break;
            case REDO_ASK_SUCCESS:
                printMessagePlayCaro(res->message);
                break;
            case YOU_WIN:
                if (readPickSuccess(res->data, username, &x, &y)) addPicked(username, x, y);
                printMessagePlayCaro(res->message);
                break;
            case OTHER_PLAYER_WIN:
                if (readPickSuccess(res->data, username, &x, &y)) addPicked(username, x, y);
                printMessagePlayCaro(res->message);
                break;
            case QUIT_SUCCESS:
                dashboard();
                break;
            case GET_USERS_SUCCESS:
                frameUserManagement();
                displayUserData();
                break;
            case GET_USERS_CONTINUE:
                char password[50], role[50];
                unsigned int wins, losses, draws;
                if (readGetUsersContinue(res->data, username, password, role, &wins, &losses, &draws)) addUserData(username, password, role, wins, losses, draws);
                break;
            case GET_USERS_FAIL:
                break;
            case ADD_USER_SUCCESS:
                break;
            case ADD_USER_FAIL:
                break;
            case EDIT_USER_SUCCESS:
                break;
            case EDIT_USER_FAIL:
                break;
            case DELETE_USER_SUCCESS:
                break;
            case DELETE_USER_FAIL:
                break;
            case DELETE_GAME_SUCCESS:
                break;
            case DELETE_GAME_FAIL:
                break;
            case GET_USERNAME_REPLAYS_SUCCESS:
                drawReplayInfoUI();
                displayReplayInfoData();
                break;
            case GET_USERNAME_REPLAYS_CONTINUE:
                char player1[50], player2[50], result[50];
                unsigned int game_id;
                if (readGetReplaysContinue(res->data, &game_id, player1, player2, result)) addReplayData(game_id, player1, player2, result);
                break;
            case GET_ALL_REPLAYS_CONTINUE:
                if (readGetReplaysContinue(res->data, &game_id, player1, player2, result)) addReplayData(game_id, player1, player2, result);
                break;
            case GET_ALL_REPLAYS_SUCCESS:
                drawReplayManagementUI();
                displayReplayData();
                break;
            case GET_REPLAYID_MOVES_SUCCESS:
                DrawReplayBoard();
                ReplayGameInfo(res->data);
                break;
            case GET_REPLAYID_MOVES_FAIL:
                break;
            default:
                break;
            }
        }
        free(res);
    }
    return 0;
}