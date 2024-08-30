#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "protocol.h"

void createSignInRequest(char *opcode, char *username, char *pass, Request *req);
void createSignupRequest(char *opcode, char *username, char *pass, char *confirm_pass, Request *req);
void createSignoutRequest(char *opcode, Request *req, char* username);
void createLogOutByXRequest(char *opcode, Request *req, char* username);
void createStartGameClientRequest(char *opcode, Request *req, char *username);
void createPickClientRequest(char *opcode, Request *req, unsigned int game_id, char* username, unsigned char x, unsigned char y);
void createQuitClientRequest(char *opcode, Request *req, char *username);
void createRedoAskRequest(char *opcode, Request *req, unsigned int game_id, char* username);
void createRedoAgreeRequest(char *opcode, Request *req, unsigned int game_id, char* username);

int signin(int clientfd, char* username, char* password);
int signup(int clientfd, char* username, char* password, char* confirm_pass);
int signout(int clientfd, char* username);
int signoutByX(int clientfd, char* username);
int startGame(int clientfd, char *username);
int pick(int clientfd, unsigned int game_id, char* username, unsigned char x, unsigned char y);
int redoAsk(int clientfd, char *username, unsigned int game_id);
int redoAgree(int clientfd, char *username, unsigned int game_id);
int quit(int clientfd, char *username);

int signup(int clientfd, char* username, char* password, char* confirm_pass) {
    Request *req = createRequest();
    createSignupRequest(STRING_SIGN_UP, username, password, confirm_pass, req);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    free(req);
    return 1;
}

int signin(int clientfd, char* username, char* password) {
    Request *req = createRequest();
    createSignInRequest(STRING_SIGN_IN, username, password, req);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return 0;
    free(req);
    return 1;
}

int signout(int clientfd, char* username) {
    Request *req = createRequest();
    createSignoutRequest(STRING_SIGN_OUT, req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    free(req);
    return 1;
}

int signoutByX(int clientfd, char* username) {
    Request *req = createRequest();
    createSignoutRequest(STRING_CLOSE, req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    free(req);
    return 1;
}

int startGame(int clientfd, char *username) {
    Request *req = createRequest();
    createStartGameClientRequest(STRING_FIND_GAME, req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    free(req);
    return 1;
}

int pick(int clientfd, unsigned int game_id, char* username, unsigned char x, unsigned char y) {
    Request *req = createRequest();
    createPickClientRequest(STRING_PICK, req, game_id, username, x, y);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    free(req);
    return 1;
}

int redoAsk(int clientfd, char *username, unsigned int game_id) {
    Request *req = createRequest();
    createRedoAskRequest(STRING_REDO_ASK, req, game_id, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    free(req);
    return 1;
}

int redoAgree(int clientfd, char *username, unsigned int game_id) {
    Request *req = createRequest();
    createRedoAgreeRequest(STRING_REDO_AGREE, req, game_id, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    free(req);
    return 1;
}

int quit(int clientfd, char *username) {
    Request *req = createRequest();
    createQuitClientRequest(STRING_QUIT, req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
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

void createPickClientRequest(char *opcode, Request *req, unsigned int game_id, char* username, unsigned char x, unsigned char y) {
    char sendbuff[MAX_LENGTH];
    snprintf(sendbuff, sizeof(sendbuff), "%s %s%c%d%c%d%c%d%c", opcode, username, '@', game_id, '@', x, '@', y, '\0');
    // strcpy(sendbuff, opcode);
    // strcat(sendbuff, " ");
    // strcat(sendbuff, username);
    setOpcodeRequest(req, sendbuff);
}

void createRedoAskRequest(char *opcode, Request *req, unsigned int game_id, char* username) {
    char sendbuff[MAX_LENGTH];
    snprintf(sendbuff, sizeof(sendbuff), "%s %s%c%d%c", opcode, username, '@', game_id, '\0');
    setOpcodeRequest(req, sendbuff);
}

void createRedoAgreeRequest(char *opcode, Request *req, unsigned int game_id, char* username) {
    char sendbuff[MAX_LENGTH];
    snprintf(sendbuff, sizeof(sendbuff), "%s %s%c%d%c", opcode, username, '@', game_id, '\0');
    setOpcodeRequest(req, sendbuff);
}

void createQuitClientRequest(char *opcode, Request *req, char *username) {
    char sendbuff[MAX_LENGTH];
    strcpy(sendbuff, opcode);
    strcat(sendbuff, " ");
    strcat(sendbuff, username);
    setOpcodeRequest(req, sendbuff);
}