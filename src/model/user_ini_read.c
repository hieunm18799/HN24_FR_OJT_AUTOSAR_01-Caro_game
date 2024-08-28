#include "users.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

void readUsersIni(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open Users.ini");
        return;
    }

    char line[MAX_LINE_LENGTH];
    User* current = NULL;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '[') {  // Mở đầu một mục người dùng mới
            current = (User*)malloc(sizeof(User));
            memset(current, 0, sizeof(User));
            sscanf(line, "[%49[^]]]", current->username);
            current->clientfd = INVALID_SOCKET; // Khởi tạo giá trị socket không hợp lệ
            current->next = userList;
            userList = current;
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
