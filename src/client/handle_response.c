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