#include "users.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

User* userList = NULL;

void initializeUser() {
    readUsersIni("Users.ini");
}

User *createUser(const char* username, const char* password, const char* role) {
    User *res = (User*)malloc(sizeof(User));
    if (res == NULL) return NULL;
    strcpy(res->username, username);
    strcpy(res->password, password);
    if (role == "") strcpy(res->role, "default");
    else strcpy(res->role, role);
    strcpy(res->status, "NOT_SIGN_IN");
    res->clientfd = INVALID_SOCKET;
    res->wins = 0;
    res->losses = 0;
    res->draws = 0;
    return res;
}

void newUser(const char* username, const char* password, const char* role) {
    User* newUser = createUser(username, password, role);
    newUser->next = userList;
    userList = newUser;
}

bool setUserStatus(const char* username, const char* status) {
    User* current = userList;
    while (current) {
        if (strcmp(current->username, username) == 0) {
            strcpy(current->status, status);
            return true;
        }
        current = current->next;
    }
    return false;
}

void deleteUser(const char* username) {
    User* current = userList;
    User* prev = NULL;
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            if (prev == NULL) {
                userList = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            writeUsersIni("Users.ini");
            return;
        }
        prev = current;
        current = current->next;
    }
}

char* getUserRole(const char* username) {
    User* current = userList;
    while (current) {
        if (strcmp(current->username, username) == 0) {
            return current->role;
        }
        current = current->next;
    }
    return "default"; // Mặc định nếu không tìm thấy người dùng
}

User* getUsers() {
    return userList;
}

User *findUserByName(const char* username) {
    User *current = userList; // `userList` là con trỏ tới danh sách người chơi
    User *res = NULL;

    // Tìm người chơi đang tìm trận dựa trên tên
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            res = current;
            break;
        }
        current = current->next;
    }
    return res;
}

void freeUsers() {
    while (userList != NULL) {
        User *temp = userList;
        userList = userList->next;
        free(temp);
    }
}