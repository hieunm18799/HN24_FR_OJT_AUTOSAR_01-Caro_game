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
bool handleFindGame(int clientfd, Request *req, Response *res);
bool handlePick(int clientfd, Request *req, Response *res);
bool handleQuit(int clientfd, Request *req, Response *res);



bool handleRedoAsk(int clientfd, Request *req, Response *res);
bool handleRedoAgree(int clientfd, Request *req, Response *res);


bool handleSignup(int clientfd, Request *req, Response *res) {
    char *username, *password, *confirmPassword;

    printf("%s\n", req->message);
    username = strtok(req->message, "@");
    password = strtok(NULL, "@");
    confirmPassword = strtok(NULL, "\0");

    res->code = sign_up(username, password, confirmPassword);
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

    res->code = sign_in(clientfd, username, password, role);
    setMessageResponse(res);

    // User not existed
    if (res->code == SIGN_IN_SUCCESS) {
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
    res->code = sign_out(clientfd, username);
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}

bool handleFindGame(int clientfd, Request *req, Response *res) {
    char *username;
    unsigned int game_id;
    User *hostPlayer = createUser("", "", "");
    User *curUser = findUserByName(username);

    printf("%s\n", req->message);
    username = strtok(req->message, "\0");
    res->code = findGame(curUser, &game_id, hostPlayer);

    if(res->code == WAITING_PLAYER) {
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return false;
    }

    res->code = GAME_START;
    setMessageResponse(res);
    sprintf(res->data, "%d", game_id);
    sprintf(res->data, "%c", '@');
    sprintf(res->data, "%s", hostPlayer->username);
    sprintf(res->data, "%c", '-');
    sprintf(res->data, "%d", hostPlayer->wins);
    sprintf(res->data, "%c", '-');
    sprintf(res->data, "%d", hostPlayer->losses);
    sprintf(res->data, "%c", '@');
    sprintf(res->data, "%s", username);
    sprintf(res->data, "%c", '-');
    sprintf(res->data, "%d", curUser->wins);
    sprintf(res->data, "%c", '-');
    sprintf(res->data, "%d", curUser->losses);
    sprintf(res->data, "%c", '\0');
    sendRes(hostPlayer->clientfd, res, sizeof(Response), 0);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}

bool handlePick(int clientfd, Request *req, Response *res) {
    char *username;
    SOCKET opofd;
    strcpy(username, strtok(req->message, "@"));
    unsigned int game_id = atoi(strtok(NULL, "@"));
    unsigned char x = atoi(strtok(NULL, "@")), y = atoi(strtok(NULL, "\0"));

    res->code = pickCaro(username, game_id, x, y, &opofd);
    setMessageResponse(res);

    if (res->code == PICK_FAIL || res->code == OTHER_PLAYER_TURN) {
        sendRes(clientfd, res, sizeof(Response), 0);
        return true;
    }

    setMessageResponse(res);
    sprintf(res->data, "%d", x);
    sprintf(res->data, "%c", '@');
    sprintf(res->data, "%d", y);
    sprintf(res->data, "%c", '\0');
    sendRes(opofd, res, sizeof(Response), 0);
    
    res->code = OTHER_PLAYER_TURN;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);  
    return true;
}

bool handleRedoAsk(int clientfd, Request *req, Response *res) {
    char *username;
    User *opoUser;
    strcpy(username, strtok(req->message, "@"));
    unsigned int game_id = atoi(strtok(NULL, "\0"));
    res->code = redoAsk(username, game_id, opoUser);
    setMessageResponse(res);
    sendRes(opoUser->clientfd, res, sizeof(Response), 0); 
    return true;
}

bool handleRedoAgree(int clientfd, Request *req, Response *res) {
    char *username;
    User *opoUser;
    strcpy(username, strtok(req->message, "@"));
    unsigned int game_id = atoi(strtok(NULL, "\0"));

    res->code = redoAgree(username, game_id, opoUser);
    setMessageResponse(res);
    sendRes(opoUser->clientfd, res, sizeof(Response), 0);
    if (res->code == REDO_FAIL) {
        return true;
    }
    res->code = OTHER_PLAYER_TURN;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}

bool handleQuit(int clientfd, Request *req, Response *res) {
    char *username;
    strcpy(username, strtok(req->message, "@"));
    unsigned int game_id = atoi(strtok(NULL, "\0"));
    
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

bool handleshowReplay(int clientfd, Request *req, Response *res)
{
    char *username;
    strcpy(username, strtok(req->message, "@"));
    MatchHistory *history;
    ReplayData *replayDataArray;
    int *numReplays;
    res->code = fetchReplayDataForDisplay(history, replayDataArray, numReplays);
 
    if (res->code == GET_REPLAYS)
    {      
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return true;
    }
}