#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "protocol.h"

void createSignInRequest(char *opcode, char *username, char *pass, Request *req);
void createSignupRequest(char *opcode, char *username, char *pass, char *confirm_pass, Request *req);
void createSignoutRequest(char *opcode, Request *req, char* username);
void createLogOutByXRequest(char *opcode, Request *req, char* username);
void createStartGameClientRequest(char *opcode, Request *req, char *username);
void createPickClientRequest(char *opcode, Request *req, char *pickedNumber);
void createQuitClientRequest(char *opcode, Request *req, char *username);

int signin(int clientfd, char* username, char* password);
int signup(int clientfd, char* username, char* password, char* confirm_pass);
int signout(int clientfd, char* username);
int signoutByX(int clientfd, char* username);
int startGame(int clientfd, char *username);
int pick(int clientfd, char *pickedNumber);
int quit(int clientfd, char *username);

int signup(int clientfd, char* username, char* password, char* confirm_pass) {
    Request *req = createRequest();
    createSignupRequest(STRING_SIGN_UP, username, password, confirm_pass, req);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    free(req);
    return 1;
}

int signin(int clientfd, char* username, char* password) {
    Request *req = createRequest();
    createSignInRequest(STRING_SIGN_IN, username, password, req);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return 0;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    free(req);
    return 1;
}

int signout(int clientfd, char* username) {
    Request *req = createRequest();
    createSignoutRequest(STRING_SIGN_OUT, req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    free(req);
    return 1;
}

int signoutByX(int clientfd, char* username) {
    Request *req = createRequest();
    createSignoutRequest(STRING_CLOSE, req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    free(req);
    return 1;
}

int startGame(int clientfd, char *username) {
    Request *req = createRequest();
    createStartGameClientRequest(STRING_FIND_GAME, req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    free(req);
    return 1;
}

int pick(int clientfd, char *pickedNumber) {
    Request *req = createRequest();
    createPickClientRequest(STRING_PICK, req, pickedNumber);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    free(req);
    return 1;
}

int quit(int clientfd, char *username) {
    Request *req = createRequest();
    createQuitClientRequest(STRING_QUIT, req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    free(req);
    return 1;
}

/*Create request*/
void createSignInRequest(char *opcode, char *username, char *pass, Request *req) {
    char sendbuff[MAX_LENGTH];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, username);
    strcat(sendbuff, "@");
    strcat(sendbuff, pass);
    setOpcodeRequest(req, sendbuff);
}

void createSignupRequest(char *opcode, char *username, char *pass, char *confirm_pass, Request *req) {
    char sendbuff[MAX_LENGTH];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, username);
    strcat(sendbuff, "@");
    strcat(sendbuff, pass);
    strcat(sendbuff, "@");
    strcat(sendbuff, confirm_pass);
    setOpcodeRequest(req, sendbuff);
}

void createSignoutRequest(char *opcode, Request *req, char* username) {
    char sendbuff[MAX_LENGTH];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, username);
    setOpcodeRequest(req, sendbuff);
}

void createLogOutByXRequest(char *opcode, Request *req, char* username) {
    char sendbuff[MAX_LENGTH];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, username);
    setOpcodeRequest(req, sendbuff);
}

void createStartGameClientRequest(char *opcode, Request *req, char *username) {
    char sendbuff[MAX_LENGTH];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, username);
    setOpcodeRequest(req, sendbuff);
}

void createPickClientRequest(char *opcode, Request *req, char *pickedNumber) {
    char sendbuff[MAX_LENGTH];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, pickedNumber);
    setOpcodeRequest(req, sendbuff);
}

void createQuitClientRequest(char *opcode, Request *req, char *username) {
    char sendbuff[MAX_LENGTH];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, username);
    setOpcodeRequest(req, sendbuff);
}