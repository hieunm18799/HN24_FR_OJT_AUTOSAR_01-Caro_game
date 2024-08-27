#include "users.h"

extern User *global_users;

User *newUser(char *username, char *password, ROLE role, USER_STATUS status, int clientfd, __uint16_t wins, __uint16_t losses, __uint16_t draws) {
    User *ret = (User*)malloc(sizeof(User));
    if (ret == NULL) {
        perror("MEMORY LEAKED!");
        return NULL;
    }
    strcpy(ret->username, username);
    strcpy(ret->password, password);
    ret->role = role;
    ret->status = status;
    ret->clientfd = clientfd;
    ret->wins = wins;
    ret->losses = losses;
    ret->draws = draws;
    ret->next = NULL;

    return ret;
}

void initializeUser() {
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL) {
        printf("Error opening INI database file.\n");
        return;
    }

    char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        while (line[0] != '[')
            fgets(line, sizeof(line), file);
        
        char key[MAX_LENGTH], value[MAX_LENGTH];
        char username[MAX_LENGTH];
        char password[MAX_LENGTH];
        ROLE role;
        USER_STATUS status;
        int clientfd;
        __uint16_t wins, losses, draws;

        // Username
        sscanf(line, "[%s[^]]]", value);
        strcpy(username, value);
        // Password
        fgets(line, sizeof(line), file);
        sscanf(line, "%s[^=]=%s[^\n]", key, value);
        strcpy(password, value);
        // Role
        fgets(line, sizeof(line), file);
        sscanf(line, "%s[^=]=%s[^\n]", value, value);
        role = atoi(value);
        // Status
        fgets(line, sizeof(line), file);
        sscanf(line, "%s[^=]=%s[^\n]", value, value);
        status = atoi(value);
        // Clientfd
        fgets(line, sizeof(line), file);
        sscanf(line, "%s[^=]=%s[^\n]", value, value);
        clientfd = atoi(value);
        // Wins
        fgets(line, sizeof(line), file);
        sscanf(line, "%s[^=]=%s[^\n]", value, value);
        wins = atoi(value);
        // Losses
        fgets(line, sizeof(line), file);
        sscanf(line, "%s[^=]=%s[^\n]", value, value);
        losses = atoi(value);
        // Draws
        fgets(line, sizeof(line), file);
        sscanf(line, "%s[^=]=%s[^\n]", value, value);
        draws = atoi(value);

        User *temp_user = newUser(username, password, role, status, clientfd, wins, losses, draws);
        temp_user->next = global_users;
        global_users = temp_user;
    }

    fclose(file);
}

__uint8_t adddUser(char *username, char *password, ROLE role) {
    if (findUser(username) != NULL) return 0;
    User *temp_user = newUser(username, password, role, NOT_SIGN_IN, -1, 0, 0, 0);
    temp_user->next = global_users;
    global_users = temp_user;
    return 1;
}

User *findUser(char *username) {
    User *temp = global_users;

    while (temp != NULL) {
        if (strcmp(temp->username, username) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

ROLE checkUser(char *username, char *password);
void changeUser(char *username, char *password);
void deleteUser(char *username);
User *getUsers();
void freeUsers();