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
    REDO_ASK,
    REDO_AGREE,
    AGREE,
    QUIT,
    // replay watch
    GET_USERNAME_REPLAYS,
    GET_REPLAYID_MOVES,
    GET_ALL_REPLAYS,
    // admin
    GET_USERS,
    ADD_USER,
    EDIT_USER,
    DELETE_USER,
    DELETE_GAME,
    REPLAY_CONTROL,
} REQ_OPCODE;

typedef enum RES_OPCODE {
    // error
    SYNTAX_ERROR,
    // login
    SIGN_UP_INPUT_WRONG,
    SIGN_IN_INPUT_WRONG,
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
    PICK_SUCCESS,
    REDO_FAIL,
    REDO_SUCCESS,
    REDO_ASK_SUCCESS,
    YOU_WIN,
    OTHER_PLAYER_WIN,
    QUIT_SUCCESS,
    // admin
    GET_USERS_SUCCESS,
    GET_USERS_CONTINUE,
    GET_USERS_FAIL,
    ADD_USER_SUCCESS,
    ADD_USER_FAIL,
    EDIT_USER_SUCCESS,
    EDIT_USER_FAIL,
    DELETE_USER_SUCCESS,
    DELETE_USER_FAIL,
    DELETE_GAME_SUCCESS,
    DELETE_GAME_FAIL,
    DELETE_REPLAY_SUCCESS,
    DELETE_REPLAY_FAILURE,
    DELETE_REPLAY_NOT_FOUND,
    //replay
    GET_USERNAME_REPLAYS_SUCCESS,
    GET_USERNAME_REPLAYS_CONTINUE,
    GET_USERNAME_REPLAYS_FAIL,
    GET_REPLAYID_MOVES_SUCCESS,
    GET_REPLAYID_MOVES_FAIL,
    GET_ALL_REPLAYS_SUCCESS,
    GET_ALL_REPLAYS_CONTINUE,
} RES_OPCODE;

// Request string
#define STRING_CLOSE "CLOSE"
#define STRING_SIGN_UP "SIGN_UP"
#define STRING_SIGN_IN "SIGN_IN"
#define STRING_SIGN_OUT "SIGN_OUT"
#define STRING_FIND_GAME "FIND_GAME"
#define STRING_PICK "PICK"
#define STRING_REDO_ASK "REDO_ASK"
#define STRING_REDO_AGREE "REDO_AGREE"
#define STRING_QUIT "QUIT"
#define STRING_GET_USERNAME_REPLAYS "GET_USERNAME_REPLAYS"
#define STRING_GET_ALL_REPLAYS "GET_ALL_REPLAYS"
#define STRING_GET_REPLAYID_MOVES "GET_REPLAYID_MOVES"
#define STRING_GET_USERS "GET_USERS"
#define STRING_ADD_USER "ADD_USER"
#define STRING_ADD_USER "ADD_USER"
#define STRING_EDIT_USER "EDIT_USER"
#define STRING_DELETE_USER "DELETE_USER"
#define STRING_DELETE_GAME "DELETE_GAME"
#define STRING_REPLAY_CONTROL "REPLAY_CONTROL"

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