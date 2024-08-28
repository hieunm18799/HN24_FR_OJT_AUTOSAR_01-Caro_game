#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include "users.h"

#define USERS_FILE "Users.ini"

void sign_up(SOCKET clientSocket) {
    char username[50];
    char password[50];
    char response[100];
    char role[50] = "default";

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

    // Kiểm tra xem tài khoản đã tồn tại chưa
    if (validateUser(username, "")) {
        snprintf(response, sizeof(response), "Username already exists.");
        send(clientSocket, response, strlen(response), 0);
        return;
    }

    // Tạo tài khoản mới
    newUser(username, password, role);
    writeUsersIni(USERS_FILE);

    snprintf(response, sizeof(response), "Account created successfully.");
    send(clientSocket, response, strlen(response), 0);
}
