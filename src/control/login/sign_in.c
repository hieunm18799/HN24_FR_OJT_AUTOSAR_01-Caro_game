#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include "users.h"

#define USERS_FILE "Users.ini"

void sign_in(SOCKET clientSocket) {
    char username[50];
    char password[50];
    char response[100];

    // Nhận dữ liệu từ client
    int recvResult = recv(clientSocket, username, sizeof(username) - 1, 0);
    if (recvResult == SOCKET_ERROR) {
        printf("Failed to receive username.\n");
        return;
    }
    username[recvResult] = '\0'; // Null-terminate the string

    recvResult = recv(clientSocket, password, sizeof(password) - 1, 0);
    if (recvResult == SOCKET_ERROR) {
        printf("Failed to receive password.\n");
        return;
    }
    password[recvResult] = '\0'; // Null-terminate the string

    // Xác thực người dùng
    if (validateUser(username, password)) {
        setUserStatus(username, "SIGN_IN");
        writeUsersIni(USERS_FILE);
        snprintf(response, sizeof(response), "Sign-in successful.");
    } else {
        snprintf(response, sizeof(response), "Invalid username or password.");
    }

    send(clientSocket, response, strlen(response), 0);
}
