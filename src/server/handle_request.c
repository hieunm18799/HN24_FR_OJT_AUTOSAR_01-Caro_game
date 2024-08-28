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

    // If Sign up input wrong: pass != conf_pass, pass == "', username = ''
    if (0) {
        res->code = SIGN_UP_INPUT_WRONG;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return false;
    }

    // If Username existed
    if (0) {
        res->code = USERNAME_EXISTED;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return false;
    }
    // add User to global_user
    // add User to ini file

    res->code = SIGN_UP_SUCCESS;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}

bool handleSignin(int clientfd, Request *req, Response *res) {
    char *username, *password;
    
    printf("%s\n", req->message);
    username = strtok(req->message, "@");
    password = strtok(NULL, "\0");

    // User not existed
    if (0) {
        res->code = USERNAME_NOT_EXISTED;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return false;
    }

    // User is ok but not pass
    if (0) {
        res->code = WRONG_PASSWORD;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return false;
    }

    // User is signed in
    if (0) {
        res->code = ACCOUNT_BUSY;
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return false;
    }

    res->code = SIGN_IN_SUCCESS;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}

bool handleSignout(int clientfd, Request *req, Response *res) {
    // Change user status to not_sign_in
    res->code = SIGN_OUT_SUCCESS;
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