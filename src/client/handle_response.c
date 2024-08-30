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