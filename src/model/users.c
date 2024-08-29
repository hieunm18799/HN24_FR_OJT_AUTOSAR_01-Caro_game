#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

User* userList = NULL;

void initializeUser() {
    readUsersIni("Users.ini");
}
bool validateUser(const char* username, const char* password) {
    User* current = userList;
    while (current) {
        if (strcmp(current->username, username) == 0 && strcmp(current->password, password) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void newUser(const char* username, const char* password, const char* role) {
    User* newUser = (User*)malloc(sizeof(User));
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    strcpy(newUser->role, role);
    strcpy(newUser->status, "NOT_SIGN_IN");
    newUser->clientfd = INVALID_SOCKET;
    newUser->wins = 0;
    newUser->losses = 0;
    newUser->draws = 0;
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
