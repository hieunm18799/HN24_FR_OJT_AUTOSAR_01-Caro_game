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
bool handleControlReplay(int clientfd, Request *req, Response *res);

bool handleRedoAsk(int clientfd, Request *req, Response *res);
bool handleRedoAgree(int clientfd, Request *req, Response *res);


bool handleSignup(int clientfd, Request *req, Response *res) {
    char *username, *password, *confirmPassword;

    printf("Message: %s\n", req->message);
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
    
    printf("Message: %s\n", req->message);
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
    char username[MAX_LENGTH];
    
    printf("Message: %s\n", req->message);
    strcpy(username, strtok(req->message, "\0"));
    res->code = sign_out(clientfd, username);
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}

bool handleFindGame(int clientfd, Request *req, Response *res) {
    char username[MAX_LENGTH];
    unsigned int game_id;
    printf("Message: %s\n", req->message);
    strcpy(username, strtok(req->message, "\0"));
    User *hostPlayer = createUser("", "", "");
    User *curUser = findUserByName(username);
    res->code = findGame(curUser, &game_id, hostPlayer);


    if(res->code == WAITING_PLAYER) {
        setMessageResponse(res);
        snprintf(res->data, sizeof(char) * MAX_LENGTH, "%d%c", game_id, '\0');
        printf("Data: %s\n", res->data);
        sendRes(clientfd, res, sizeof(Response), 0);
        return true;
    }

    res->code = GAME_START;
    setMessageResponse(res);
    snprintf(res->data, sizeof(char) * MAX_LENGTH, "%d%c%s%c%d%c%d%c%s%c%d%c%d%c", game_id, '@', hostPlayer->username, '-', hostPlayer->wins, '-', hostPlayer->losses, '@', username, '-', curUser->wins, '-',  curUser->losses, '\0');
    printf("Data: %s\n", res->data);
    sendRes(hostPlayer->clientfd, res, sizeof(Response), 0);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}

bool handlePick(int clientfd, Request *req, Response *res) {
    printf("Message: %s\n", req->message);
    char username[MAX_LENGTH];
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
    snprintf(res->data, sizeof(char) * MAX_LENGTH, "%s%c%d%c%d%c", username, '@', x, '@', y, '\0');
    // sprintf(res->data, "%d", x);
    // sprintf(res->data, "%c", '@');
    // sprintf(res->data, "%d", y);
    // sprintf(res->data, "%c", '\0');
    sendRes(clientfd, res, sizeof(Response), 0);
    sendRes(opofd, res, sizeof(Response), 0);
    
    res->code = OTHER_PLAYER_TURN;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);  
    res->code = YOUR_TURN;
    setMessageResponse(res);
    sendRes(opofd, res, sizeof(Response), 0);  
    return true;
}

bool handleRedoAsk(int clientfd, Request *req, Response *res) {
    char username[MAX_LENGTH];
    SOCKET opofd;
    strcpy(username, strtok(req->message, "@"));
    unsigned int game_id = atoi(strtok(NULL, "\0"));
    res->code = redoAsk(username, game_id, &opofd);
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    if (res->code == REDO_ASK_SUCCESS){
        setMessageResponse(res);
        sendRes(opofd, res, sizeof(Response), 0);
    }
    return true;
}

bool handleRedoAgree(int clientfd, Request *req, Response *res) {
    char username[MAX_LENGTH];
    SOCKET opofd;
    strcpy(username, strtok(req->message, "@"));
    unsigned int game_id = atoi(strtok(NULL, "\0"));

    res->code = redoAgree(username, game_id, &opofd);
    if (res->code == REDO_FAIL) {
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return true;
    }
    setMessageResponse(res);
    sendRes(opofd, res, sizeof(Response), 0);
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    res->code = OTHER_PLAYER_TURN;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);  
    res->code = YOUR_TURN;
    setMessageResponse(res);
    sendRes(opofd, res, sizeof(Response), 0);  
    return true;
}

bool handleQuit(int clientfd, Request *req, Response *res) {
    char username[MAX_LENGTH];
    SOCKET opofd = SOCKET_ERROR;
    strcpy(username, strtok(req->message, "@"));
    unsigned int game_id = atoi(strtok(NULL, "\0"));

    res->code = quitLogic(username, game_id, &opofd);
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);  
    
    if (opofd != SOCKET_ERROR) {
        res->code = YOU_WIN;
        setMessageResponse(res);
        sendRes(opofd, res, sizeof(Response), 0);  
    }
    return true;
}

bool handleshowReplay(int clientfd, Request *req, Response *res)
{
    char username[MAX_LENGTH];
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

bool handleControlReplay(int clientfd, Request *req, Response *res)
{
    char username[MAX_LENGTH];
    strcpy(username, strtok(req->message, "@"));
    MatchHistory** head;
    int *game_id;
    res->code = fetchdeleteReplay(head, game_id);

    if (res->code == REPLAY_CONTROL)
    {      
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return true;
    }
}
