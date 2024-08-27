#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "protocol.h"

void inputRequest(char *sendbuff);
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
int startGameClient(int clientfd, char *username);
int pickClient(int clientfd, char *pickedNumber);
int quit(int clientfd, char *username);

void inputRequest(char *sendbuff) {
    printf("Please enter the message: ");
    fflush(stdin);
    fgets(sendbuff,MAX_LENGTH,stdin); //enter data
    sendbuff[strlen(sendbuff) - 1] = '\0';
}

int signup(int clientfd, char* username, char* password, char* confirm_pass) {
    Request *req = (Request *)malloc(sizeof(Request));
    createSignupRequest("REGISTER", username, password, confirm_pass, req);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    return 1;
}

int signin(int clientfd, char* username, char* password) {
    Request *req = (Request *)malloc(sizeof(Request));
    createSignInRequest("LOGIN", username, password, req);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return 0;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    return 1;
}

int signout(int clientfd, char* username) {
    Request *req = (Request *)malloc(sizeof(Request));
    createSignoutRequest("LOGOUT", req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    return 1;
}

int signoutByX(int clientfd, char* username) {
    Request *req = (Request *)malloc(sizeof(Request));
    createSignoutRequest("CLOSE", req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    return 1;
}

int startGameClient(int clientfd, char *username) {
    Request *req = (Request *)malloc(sizeof(Request));
    createStartGameClientRequest("PLAY", req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    return 1;
}

int pickClient(int clientfd, char *pickedNumber) {
    Request *req = (Request *)malloc(sizeof(Request));
    createPickClientRequest("PICK", req, pickedNumber);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
    return 1;
}

int quit(int clientfd, char *username) {
    Request *req = (Request *)malloc(sizeof(Request));
    createQuitClientRequest("QUIT", req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    printf("Sent %d bytes to server\n", n_sent);
    printf("Waiting for reply\n");
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