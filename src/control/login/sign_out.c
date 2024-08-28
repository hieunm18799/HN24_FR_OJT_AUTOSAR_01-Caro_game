#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include "user.h"

#define USERS_FILE "Users.ini"

void sign_out(SOCKET clientSocket) {
    char username[50];
    char response[100];

    // Nhận dữ liệu từ client
    int recvResult = recv(clientSocket, username, sizeof(username) - 1, 0);
    if (recvResult == SOCKET_ERROR) {
        printf("Failed to receive username.\n");
        return;
    } 
    username[recvResult] = '\0'; // Null-terminate the string

    // Cập nhật trạng thái người dùng
    if (setUserStatus(username, "NOT_SIGN_IN")) {
        writeUsersIni(USERS_FILE);
        snprintf(response, sizeof(response), "Sign-out successful.");
    } else {
        snprintf(response, sizeof(response), "Error signing out. Please check the username.");
    }

    send(clientSocket, response, strlen(response), 0);
}
