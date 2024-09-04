#include "protocol.h"

void readSigninSuccess(char *input, char *username, char *role){
    strcpy(username, strtok(input, "@"));
    strcpy(role, strtok(NULL, "\0"));
    // int i, usernameLength = 0, roleLength = 0;
    // for (i = 0; input[i] != '@'; i++){
    //     username[usernameLength++] = input[i];
    // }
    // username[usernameLength] = '\0';
    // i++;
    // for (; i < strlen(input); i++){
    //     role[roleLength++] = input[i];
    // }
    // role[roleLength] = '\0';
}

void readWaitingGame(char *input, unsigned int *game_id){
    *game_id = atoi(strtok(input, "\0"));
}

void readGameStart(char *input, unsigned int *game_id, char* player1_username, int *player1_win, int* player1_lose, char* player2_username, int *player2_win, int* player2_lose){
    *game_id = atoi(strtok(input, "@"));
    strcpy(player1_username, strtok(NULL, "-"));
    *player1_win = atoi(strtok(NULL, "-"));
    *player1_lose = atoi(strtok(NULL, "@"));
    strcpy(player2_username, strtok(NULL, "-"));
    *player2_win = atoi(strtok(NULL, "-"));
    *player2_lose = atoi(strtok(NULL, "\0"));
}

int readPickSuccess(char *input, char *username, unsigned char *x, unsigned char *y){
    if (strcmp(input, "\0") == 0) return 0;
    strcpy(username, strtok(input, "@"));
    *x = atoi(strtok(NULL, "@"));
    *y = atoi(strtok(NULL, "\0"));
    return 1;
}

int readRedoSuccess(char *input, unsigned char *x, unsigned char *y){
    if (strcmp(input, "\0") == 0) return 0;
    *x = atoi(strtok(input, "@"));
    *y = atoi(strtok(NULL, "\0"));
    return 1;
}

int readGetUsersContinue(char *input, char *username, char *password, char *role, unsigned int *wins, unsigned int *losses, unsigned int *draws){
    if (strcmp(input, "\0") == 0) return 0;
    strcpy(username, strtok(input, "@"));
    strcpy(password, strtok(NULL, "@"));
    strcpy(role, strtok(NULL, "@"));
    *wins = atoi(strtok(NULL, "@"));
    *losses = atoi(strtok(NULL, "@"));
    *draws = atoi(strtok(NULL, "\0"));
    return 1;
}

int readGetReplaysContinue(char *input, unsigned int *game_id, char *player1, char *player2, char *result){
    if (strcmp(input, "\0") == 0) return 0;
    *game_id = atoi(strtok(input, "@"));
    strcpy(player1, strtok(NULL, "@"));
    strcpy(player2, strtok(NULL, "@"));
    strcpy(result, strtok(NULL, "\0"));
    printf("%d %s %s %s\n", *game_id, player1, player2, result);
    return 1;
}