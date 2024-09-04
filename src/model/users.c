#include "users.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

User* userList = NULL;

void initializeUser() {
    readUsersIni("Users.ini");
}

User *createUser(const char* username, const char* password, const char* role, unsigned int wins, unsigned int losses, unsigned int draws) {
    User *res = (User*)malloc(sizeof(User));
    if (res == NULL) return NULL;
    strcpy(res->username, username);
    strcpy(res->password, password);
    if (role == NULL) strcpy(res->role, "default");
    else strcpy(res->role, role);
    strcpy(res->status, "NOT_SIGN_IN");
    res->clientfd = INVALID_SOCKET;
    res->wins = wins;
    res->losses = losses;
    res->draws = draws;
    res->next = NULL;
    return res;
}

void newUser(const char* username, const char* password, const char* role, unsigned int wins, unsigned int losses, unsigned int draws) {
    User* newUser = createUser(username, password, role, wins, losses, draws);
    if (userList == NULL) {
        userList = newUser;
    } else {
        User *temp = userList;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newUser;
    }
}

void changeUser(User *user, const char* username, const char* password, const char* role, unsigned int wins, unsigned int losses, unsigned int draws) {
    strcpy(user->username, username);
    strcpy(user->password, password);
    strcpy(user->role, role);
    user->wins = wins;
    user->losses = losses;
    user->draws = draws;
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

void increasedWins(User *user) {
    user->wins++;
}

void increasedLosses(User *user) {
    user->losses++;
}

int deleteUser(const char* username) {
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
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 0;
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

void writeUsersIni() {
    FILE *file = fopen(userFileName, "w");
    if (!file) {
        printf("Failed to open User's file!");
        return;
    }

    User* current = userList;
    while (current) {
        fprintf(file, "[%s]\n", current->username);
        fprintf(file, "Password=%s\n", current->password);
        fprintf(file, "Role=%s\n", current->role);
        fprintf(file, "Status=%s\n", current->status);
        fprintf(file, "Wins=%d\n", current->wins);
        fprintf(file, "Losses=%d\n", current->losses);
        fprintf(file, "Draws=%d\n", current->draws);
        fprintf(file, "\n");
        current = current->next;
    }

    fclose(file);
}

void readUsersIni() {
    FILE *file = fopen(userFileName, "r");
    if (!file) {
        printf("Failed to open User's file!");
        return;
    }

    char line[MAX_LINE_LENGTH];
    User* current = NULL, *temp;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '[') {  // Mở đầu một mục người dùng mới
            current = (User*)malloc(sizeof(User));
            memset(current, 0, sizeof(User));
            sscanf(line, "[%49[^]]]", current->username);
            current->clientfd = INVALID_SOCKET; // Khởi tạo giá trị socket không hợp lệ
            if (userList == NULL) {
                userList = current;
                temp = userList;
            } else {
                temp->next = current;
                temp = temp->next;
            }
        } else if (current) {
            if (strncmp(line, "Password=", 9) == 0) {
                sscanf(line + 9, "%49s", current->password);
            } else if (strncmp(line, "Role=", 5) == 0) {
                sscanf(line + 5, "%49s", current->role);
            } else if (strncmp(line, "Status=", 7) == 0) {
                sscanf(line + 7, "%49s", current->status);
            } else if (strncmp(line, "Wins=", 5) == 0) {
                sscanf(line + 5, "%d", &current->wins);
            } else if (strncmp(line, "Losses=", 7) == 0) {
                sscanf(line + 7, "%d", &current->losses);
            } else if (strncmp(line, "Draws=", 6) == 0) {
                sscanf(line + 6, "%d", &current->draws);
            }
        }
    }

    fclose(file);
}

void logoutUsers() {
    User *current = userList; // `userList` là con trỏ tới danh sách người chơi

    // Tìm người chơi đang tìm trận dựa trên tên
    while (current != NULL) {
        strcpy(current->status, "NOT_SIGN_IN");
        current = current->next;
    }
}