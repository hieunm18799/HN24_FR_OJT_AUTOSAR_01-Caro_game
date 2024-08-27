#include "user.h"
#include <stdio.h>
#include <stdlib.h>

void writeUsersIni(const char* filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open Users.ini");
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
