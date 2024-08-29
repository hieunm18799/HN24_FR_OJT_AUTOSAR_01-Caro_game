#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "protocol.h"
#include "users.h"
#include "games.h"

bool handleSignup(int clientfd, Request *req, Response* res);
bool handleSignin(int clientfd, Request *req, Response *res);
bool handleSignout(int clientfd, Request *req, Response *res);
bool handleFindGame(int sockfd, int clientfd, Request *req, Response *res);
bool handlePick(int clientfd, Request *req, Response *res);
bool handleQuit(int clientfd, Request *req, Response *res);

bool handleSignup(int clientfd, Request *req, Response *res) {
    char *username, *password, *confirmPassword;

    printf("%s\n", req->message);
    username = strtok(req->message, "@");
    password = strtok(NULL, "@");
    confirmPassword = strtok(NULL, "\0");

    RES_OPCODE ret = sign_up(username, password, confirmPassword);
    res->code = ret;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}

bool handleSignin(int clientfd, Request *req, Response *res) {
    char *username, *password;
    char role[MAX_LENGTH];
    
    printf("%s\n", req->message);
    username = strtok(req->message, "@");
    password = strtok(NULL, "\0");

    RES_OPCODE ret = sign_in(clientfd, username, password, role);
    res->code = ret;
    setMessageResponse(res);

    // User not existed
    if (ret == SIGN_IN_SUCCESS) {
        strcpy(res->data, username);
        strcat(res->data, "@");
        strcat(res->data, role);
        strcat(res->data, "\0");
    }
    printf("%s %s\n", res->message, res->data);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}

bool handleSignout(int clientfd, Request *req, Response *res) {
    char *username;
    
    printf("%s\n", req->message);
    username = strtok(req->message, "\0");
    RES_OPCODE ret = sign_out(clientfd, username);
    res->code = ret;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}

bool handleFindGame(int sockfd, int clientfd, Request *req, Response *res) {
    // Find game not play yet
    // if no game than create a game (user = player1) and wait for user to join with 
    if(0) {
        res->code = WAITING_PLAYER;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return false;
    }
    // The previous should return the player1's clientfd and game id
    // Need response to the orther waitinng player
    unsigned int gameid;
    int client2fd;

    res->code = GAME_START;
    setMessageResponse(res);
    sprintf(res->data, "%d", gameid);
    sendRes(clientfd, res, sizeof(Response), 0);
    sendRes(client2fd, res, sizeof(Response), 0);
    return true;
}

bool handlePick(int clientfd, Request *req, Response *res) {
    char *username;
    strcpy(username, strtok(req->message, "@"));
    unsigned int gameid = atoi(strtok(NULL, "@"));
    unsigned char x = atoi(strtok(NULL, "@")), y = atoi(strtok(NULL, "\0"));

    // Check current turn
    if (0) {
        res->code = OTHER_PLAYER_TURN;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return false;
    }

    // save (x,y)
    
    // Event if pick fail ?
    // if (0) {
    //     res->code = PICK_FAIL;
    //     setMessageResponse(res);
    //     sendRes(clientfd, res, sizeof(Response), 0);
    //     return false;
    // }

    res->code = PICK_SUCCESS;
    sprintf(res->data, "%s", req->message);
    setMessageResponse(res);

    int client2fd; // get player2 clientfd
    // Send pick success
    sendRes(clientfd, res, sizeof(Response), 0);
    sendRes(client2fd, res, sizeof(Response), 0);

    // Change player turn
    res->code = YOUR_TURN;
    setMessageResponse(res);
    sendRes(client2fd, res, sizeof(Response), 0);
    res->code = OTHER_PLAYER_TURN;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);  
    return true;
}

bool handleQuit(int clientfd, Request *req, Response *res) {
    char *username;
    strcpy(username, strtok(req->message, "@"));
    unsigned int gameid = atoi(strtok(NULL, "\0"));
    bool endGame = false;
    int client2fd;
    // Check if game is end or not
    // If not
    // Set game to end, other player won and this player lost, return client2fd
    if (endGame) {
        res->code = YOU_WIN;
        setMessageResponse(res);
        sendRes(client2fd, res, sizeof(Response), 0);  
        return true;
    }
    return true;
}