#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif

#ifdef linux
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h> 
#include <netinet/in.h>
#include <unistd.h>
#endif

#define MAX_LENGTH 256
#define BOARD_LENGTH 10

typedef enum REQ_OPCODE {
    // close terminal event
    CLOSE,
    // login
    SIGN_UP,
    SIGN_IN,
    SIGN_OUT,
    // caro play
    FIND_GAME,
    PICK,
    REDO,
    QUIT,
    // replay watch
    GET_REPLAYS,
    // admin
    GET_USERS,
    GET_GAMES,
    ADD_USER,
    EDIT_USER,
    DELETE_USER,
    DELETE_GAME,
} REQ_OPCODE;

typedef enum RES_OPCODE {
    // error
    SYNTAX_ERROR,
    // login
    SIGN_UP_INPUT_WRONG,
    USERNAME_NOT_EXISTED,
    WRONG_PASSWORD,
    USERNAME_EXISTED,
    ACCOUNT_BUSY,
    SIGN_OUT_FAIL,
    SIGN_IN_SUCCESS,
    SIGN_UP_SUCCESS,
    SIGN_OUT_SUCCESS,
    // caro play
    WAITING_PLAYER,
    GAME_START,
    YOUR_TURN,
    OTHER_PLAYER_TURN,
    PICK_FAIL,
    REDO_SUCCESS,
    REDO_FAIL,
    AGREE_REDO,
    YOU_WIN,
    OTHER_PLAYER_WIN,
    QUIT_SUCCESS,
    // admin
    GET_USERS_SUCCESS,
    GET_USERS_FAIL,
    GET_GAMES_SUCCESS,
    GET_GAMES_FAIL,
    ADD_USER_SUCCESS,
    ADD_USER_FAIL,
    EDIT_USER_SUCCESS,
    EDIT_USER_FAIL,
    DELETE_USER_SUCCESS,
    DELETE_USER_FAIL,
    DELETE_GAME_SUCCESS,
    DELETE_GAME_FAIL,
    //replay
    GET_REPLAY_SUCCESS;
} RES_OPCODE;

// Request string
#define STRING_CLOSE "CLOSE"
#define STRING_SIGN_UP "SIGN_UP"
#define STRING_SIGN_IN "SIGN_IN"
#define STRING_SIGN_OUT "SIGN_OUT"
#define STRING_FIND_GAME "FIND_GAME"
#define STRING_PICK "PICK"
#define STRING_QUIT "QUIT"
#define STRING_GET_REPLAYS "GET_REPLAYS"
#define STRING_GET_USERS "GET_USERS"
#define STRING_GET_GAMES "GET_GAMES"

// Response string
#define STRING_SYNTAX_ERROR "SYNTAX_ERROR"
#define STRING_SIGN_UP_INPUT_WRONG "SIGN_UP_INPUT_WRONG"
#define STRING_USERNAME_NOT_EXISTED "USERNAME_NOT_EXISTED"
#define STRING_WRONG_PASSWORD "WRONG_PASSWORD"
#define STRING_USERNAME_EXISTED "USERNAME_EXISTED"
#define STRING_ACCOUNT_BUSY "ACCOUNT_BUSY"
#define STRING_SIGN_IN_SUCCESS "SIGN_IN_SUCCESS"
#define STRING_SIGN_UP_SUCCESS "SIGN_UP_SUCCESS"
#define STRING_SIGN_OUT_SUCCESS "SIGN_OUT_SUCCESS"
#define STRING_WAITING_PLAYER "WAITING_PLAYER"
#define STRING_GAME_START "GAME_START"
#define STRING_YOUR_TURN "YOUR_TURN"
#define STRING_OTHER_PLAYER_TURN "OTHER_PLAYER_TURN"
#define STRING_PICK_FAIL "PICK_FAIL"
#define STRING_YOU_WIN "YOU_WIN"
#define STRING_OTHER_PLAYER_WIN "OTHER_PLAYER_WIN"
#define STRING_QUIT_SUCCESS "QUIT_SUCCESS"
#define STRING_GAME_FOUND "GAME_FOUND"
#define STRING_GAME_NOT_FOUND "GAME_NOT_FOUND"
#define STRING_REDO_SUCCESS "REDO_SUCCESS"
#define STRING_REDO_FAIL "REDO_FAIL"
#define STRING_AGREE_REDO "AGREE_REDO"
#define STRING_DELETE_GAME_SUCCESS "DELETE_GAME_SUCCESS"
#define STRING_DELETE_GAME_FAIL "DELETE_GAME_FAIL"
#define STRING_DELETE_USER_SUCCESS "DELETE_USER_SUCCESS"
#define STRING_DELETE_USER_FAIL "DELETE_USER_FAIL"
#define STRING_EDIT_USER_SUCCESS "EDIT_USER_SUCCESS"
#define STRING_EDIT_USER_FAIL "EDIT_USER_FAIL"
#define STRING_ADD_USER_SUCCESS "ADD_USER_SUCCESS"
#define STRING_ADD_USER_FAIL "ADD_USER_FAIL"
#define STRING_GET_GAMES_SUCCESS "GET_GAMES_SUCCESS"
#define STRING_GET_GAMES_FAIL "GET_GAMES_FAIL"
#define STRING_GET_USERS_SUCCESS "GET_USERS_SUCCESS"
#define STRING_GET_USERS_FAIL "GET_USERS_FAIL"

typedef struct Request {
    char message[MAX_LENGTH];
    REQ_OPCODE code;
} Request;

typedef struct Response {
    char message[MAX_LENGTH];
    char data[MAX_LENGTH];
    RES_OPCODE code;
} Response;

Request *createRequest();
Response *createResponse();

int sendReq(int socket, Request *buff, int size, int flags);
int recvReq(int socket, Request *buff, int size, int flags); 

int sendRes(int socket, Response *msg, int size, int flags);
int recvRes(int socket, Response *msg, int size, int flags);

void setMessageResponse(Response *msg);

void setOpcodeRequest(Request *request, char *input);

void splitMessage(char *input, char *code, char *data);

#endif
