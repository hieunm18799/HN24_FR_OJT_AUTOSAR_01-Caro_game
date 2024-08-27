#ifndef USERS_H
#define USERS_H

#include <string.h>
#include "protocol.h"

#define USER_FILE "src/model/Users.ini"

typedef enum USER_STATUS {
    NOT_SIGN_IN,
    SIGNED_IN,
    PLAYING,
} USER_STATUS;

typedef enum ROLE {
    USER,
    ADMIN,
} ROLE;

typedef struct User {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    ROLE role;
    USER_STATUS status;
    int clientfd;
    __uint16_t wins, losses, draws;
    struct User *next;
} User;

User *newUser(char *username, char *password, ROLE role, USER_STATUS status, int clientfd, __uint16_t wins, __uint16_t losses, __uint16_t draws);
void initializeUser();
User *findUser(char *username);
__uint8_t adddUser(char *username, char *password, ROLE role);
ROLE checkUser(char *username, char *password);
void changeUser(char *username, char *password);
void deleteUser(char *username);
User *getUsers();
void freeUsers();

#endif