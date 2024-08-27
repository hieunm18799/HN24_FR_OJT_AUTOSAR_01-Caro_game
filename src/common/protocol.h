#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_LENGTH 255
typedef enum REQ_OPCODE{
    // close terminal event
    CLOSE,
    //login
    SIGN_UP,
    SIGN_IN,
    SIGN_OUT,
    // caro play
    FIND_GAME,
    PICK,
    QUIT,
    // replay watch
    GET_REPLAYS,
    // admin
    GET_USERS,
    GET_GAMES
} REQ_OPCODE; 

typedef enum RES_OPCODE{
    // error
    SYNTAX_ERROR,
    // login
    SIGN_UP_INPUT_WRONG,
    USERNAME_NOT_EXISTED,
    WRONG_PASSWORD,
    USERNAME_EXISTED,
    ACCOUNT_BUSY,
    SIGN_IN_SUCCESS,
    SIGN_UP_SUCCESS,
    SIGN_OUT_SUCCESS,
    DISCONNECTED,
    // caro play
    WAITING_PLAYER,
    GAME_START,
    YOUR_TURN,
    OTHER_PLAYER_TURN,
    PICK_FAIL,
    PICK_SUCCESS,
    YOU_WIN,
    OTHER_PLAYER_WIN,
} RES_OPCODE;

typedef struct Request {
    REQ_OPCODE code;
    char message[MAX_LENGTH];
} Request;

typedef struct Response {
    RES_OPCODE code;
    char message[MAX_LENGTH];
    char data[MAX_LENGTH];
} Response;

int sendReq(int socket, Request *buff, int size, int flags);
int recvReq(int socket, Request *buff, int size, int flags); 

int sendRes(int socket, Response *msg, int size, int flags);
int recvRes(int socket, Response *msg, int size, int flags);

void setMessageResponse(Response *msg);
void readMessageResponse(Response *msg);

void setOpcodeRequest(Request *request, char *input);

void splitMessage(char *input, char *code, char *data);

#endif