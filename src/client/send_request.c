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
void createQuitClientRequest(char *opcode, Request *req, unsigned int game_id, char *username);
void createRedoAskRequest(char *opcode, Request *req, unsigned int game_id, char* username);
void createRedoAgreeRequest(char *opcode, Request *req, unsigned int game_id, char* username);
void createGetUsersDataRequest(char *opcode, Request *req);
void createAdminAddUserRequest(char *opcode, Request *req, char *username, char *password, char *role, unsigned int wins, unsigned int losses, unsigned int draws);
void createAdminEditUserRequest(char *opcode, Request *req, char *username, char *password, char *role, unsigned int wins, unsigned int losses, unsigned int draws);
void createAdminDeleteUserRequest(char *opcode, Request *req, char *username);
void createGetUsernameReplaysDataRequest(char *opcode, Request *req, char *username);
void createWatchReplay(char *opcode, Request *req, unsigned int replay_id);
void createAdminDeleteReplayRequest(char* opcode, Request* req, unsigned int replay_id, char* username);
void createGetAllReplayDataRequest(char *opcode, Request *req);

int signin(int clientfd, char* username, char* password);
int signup(int clientfd, char* username, char* password, char* confirm_pass);
int signout(int clientfd, char* username);
int signoutByX(int clientfd, char* username);
int startGame(int clientfd, char *username);
int pick(int clientfd, unsigned int game_id, char* username, unsigned char x, unsigned char y);
int redoAsk(int clientfd, char *username, unsigned int game_id);
int redoAgree(int clientfd, char *username, unsigned int game_id);
int quit(int clientfd, unsigned int game_id, char *username);
int getUsersData(int clientfd);
int adminAddUser(int clientfd, char *username, char *password, char *role, unsigned int wins, unsigned int losses, unsigned int draws);
int adminEditUser(int clientfd, char *username, char *password, char *role, unsigned int wins, unsigned int losses, unsigned int draws);
int adminDeleteUser(int clientfd, char *username);
int getUsernameReplaysData(int clientfd, char *username);
int watchReplay(int clientfd, unsigned int replay_id);
int adminDeleteReplay(int clientfd, unsigned int replay_id, char* username);
int getAllReplayData(int clientfd);

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

int quit(int clientfd, unsigned int game_id, char *username) {
    Request *req = createRequest();
    createQuitClientRequest(STRING_QUIT, req, game_id, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    free(req);
    return 1;
}

int getUsersData(int clientfd) {
    Request *req = createRequest();
    createGetUsersDataRequest(STRING_GET_USERS, req);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    free(req);
    return 1;
}

int getUsernameReplaysData(int clientfd, char *username) {
    Request *req = createRequest();
    createGetUsernameReplaysDataRequest(STRING_GET_USERNAME_REPLAYS, req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    free(req);
    return 1;
}

int adminAddUser(int clientfd, char *username, char *password, char *role, unsigned int wins, unsigned int losses, unsigned int draws) {
    Request *req = createRequest();
    createAdminAddUserRequest(STRING_ADD_USER, req, username, password, role, wins, losses, draws);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    free(req);
    return 1;
}
int adminEditUser(int clientfd, char *username, char *password, char *role, unsigned int wins, unsigned int losses, unsigned int draws) {
    Request *req = createRequest();
    createAdminEditUserRequest(STRING_EDIT_USER, req, username, password, role, wins, losses, draws);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    free(req);
    return 1;
}

int adminDeleteUser(int clientfd, char *username) {
    Request *req = createRequest();
    createAdminDeleteUserRequest(STRING_DELETE_USER, req, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    free(req);
    return 1;
}

int watchReplay(int clientfd, unsigned int replay_id) {
    Request *req = createRequest();
    createWatchReplay(STRING_GET_REPLAYID_MOVES, req, replay_id);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    free(req);
    return 1;
}
int adminDeleteReplay(int clientfd, unsigned int game_id, char* username) {
    Request* req = createRequest();
    createAdminDeleteReplayRequest(STRING_DELETE_REPLAY, req, game_id, username);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    free(req);
    return 1;
}

int getAllReplayData(int clientfd){
    Request *req = createRequest();
    createGetAllReplayDataRequest(STRING_GET_ALL_REPLAYS, req);
    int n_sent = sendReq(clientfd, req, sizeof(Request), 0);
    if (n_sent < 0)
        return n_sent;
    free(req);
    return 1;
}

////////////////////////////////////////////////////////
/*Create request*/
////////////////////////////////////////////////////////
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

void createQuitClientRequest(char *opcode, Request *req, unsigned int game_id, char *username) {
    char sendbuff[MAX_LENGTH];
    // strcpy(sendbuff, opcode);
    // strcat(sendbuff, " ");
    // strcat(sendbuff, username);
    snprintf(sendbuff, sizeof(sendbuff), "%s %s%c%d%c", opcode, username, '@', game_id, '\0');
    setOpcodeRequest(req, sendbuff);
}

void createGetUsersDataRequest(char *opcode, Request *req) {
    char sendbuff[MAX_LENGTH];
    snprintf(sendbuff, sizeof(sendbuff), "%s Admin get users!%c", opcode, '\0');
    setOpcodeRequest(req, sendbuff);
}

void createAdminAddUserRequest(char *opcode, Request *req, char *username, char *password, char *role, unsigned int wins, unsigned int losses, unsigned int draws) {
    char sendbuff[MAX_LENGTH];
    snprintf(sendbuff, sizeof(sendbuff), "%s %s%c%s%c%s%c%d%c%d%c%d%c", opcode, username, '@', password, '@', role, '@', wins, '@', losses, '@', draws, '\0');
    setOpcodeRequest(req, sendbuff);
}

void createAdminEditUserRequest(char *opcode, Request *req, char *username, char *password, char *role, unsigned int wins, unsigned int losses, unsigned int draws) {
    char sendbuff[MAX_LENGTH];
    snprintf(sendbuff, sizeof(sendbuff), "%s %s%c%s%c%s%c%d%c%d%c%d%c", opcode, username, '@', password, '@', role, '@', wins, '@', losses, '@', draws, '\0');
    setOpcodeRequest(req, sendbuff);
}

void createAdminDeleteUserRequest(char *opcode, Request *req, char *username) {
    char sendbuff[MAX_LENGTH];
    snprintf(sendbuff, sizeof(sendbuff), "%s %s%c", opcode, username, '\0');
    setOpcodeRequest(req, sendbuff);
}

void createGetUsernameReplaysDataRequest(char *opcode, Request *req, char *username) {
    char sendbuff[MAX_LENGTH];
    snprintf(sendbuff, sizeof(sendbuff), "%s %s%c", opcode, username, '\0');
    setOpcodeRequest(req, sendbuff);
}

void createWatchReplay(char *opcode, Request *req, unsigned int replay_id) {
    char sendbuff[MAX_LENGTH];
    snprintf(sendbuff, sizeof(sendbuff), "%s %d%c", opcode, replay_id, '\0');
    setOpcodeRequest(req, sendbuff);
}
void createAdminDeleteReplayRequest(char* opcode, Request* req, unsigned int replay_id, char* username) {
    char sendbuff[MAX_LENGTH];
    snprintf(sendbuff, sizeof(sendbuff), "%s %s%c%d%c", opcode, username, '@', replay_id, '\0');
    setOpcodeRequest(req, sendbuff);
}

void createGetAllReplayDataRequest(char *opcode, Request *req){
    char sendbuff[MAX_LENGTH];
    snprintf(sendbuff, sizeof(sendbuff), "%s Admin get all data replays!%c", opcode, '\0');
    setOpcodeRequest(req, sendbuff);
}