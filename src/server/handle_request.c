#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "protocol.h"
#include "users.h"
#include "games.h"

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
    char *username;
    
    printf("Message: %s\n", req->message);
    username = strtok(req->message, "\0");
    res->code = sign_out(clientfd, username);
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}

bool handleFindGame(int clientfd, Request *req, Response *res) {
    char *username;
    unsigned int game_id;
    printf("Message: %s\n", req->message);
    username = strtok(req->message, "\0");
    User *hostPlayer = createUser("", "", "", 0, 0, 0);
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

    res->code = YOUR_TURN;
    setMessageResponse(res);
    sendRes(hostPlayer->clientfd, res, sizeof(Response), 0);  
    res->code = OTHER_PLAYER_TURN;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);  
    return true;
}

bool handlePick(int clientfd, Request *req, Response *res) {
    printf("Message: %s\n", req->message);
    char *username;
    SOCKET opofd;
    username = strtok(req->message, "@");
    unsigned int game_id = atoi(strtok(NULL, "@"));
    unsigned char x = atoi(strtok(NULL, "@")), y = atoi(strtok(NULL, "\0"));

    res->code = pickCaro(username, game_id, x, y, &opofd);
    printf("pick %d\n", res->code);
    setMessageResponse(res);

    if (res->code == PICK_FAIL || res->code == OTHER_PLAYER_TURN) {
        sendRes(clientfd, res, sizeof(Response), 0);
        return true;
    }

    snprintf(res->data, sizeof(char) * MAX_LENGTH, "%s%c%d%c%d%c", username, '@', x, '@', y, '\0');
    sendRes(clientfd, res, sizeof(Response), 0);
    if (res->code == YOU_WIN) {
        res->code = OTHER_PLAYER_WIN;
        setMessageResponse(res);
        sendRes(opofd, res, sizeof(Response), 0);
        return true;
    }
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
    char *username;
    SOCKET opofd;
    username = strtok(req->message, "@");
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
    char *username;
    SOCKET opofd;
    unsigned char x, y;
    username = strtok(req->message, "@");
    unsigned int game_id = atoi(strtok(NULL, "\0"));

    res->code = redoAgree(username, game_id, &opofd, &x, &y);
    if (res->code == REDO_FAIL) {
        setMessageResponse(res);
        sendRes(clientfd, res, sizeof(Response), 0);
        return true;
    }
    setMessageResponse(res);
    snprintf(res->data, sizeof(char) * MAX_LENGTH, "%d%c%d%c", x, '@', y, '\0');
    sendRes(opofd, res, sizeof(Response), 0);
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
    char *username;
    SOCKET opofd = SOCKET_ERROR;
    username = strtok(req->message, "@");
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

bool handleControlReplay(int clientfd, Request* req, Response* res)
{
    printf("Message: %s\n", req->message);
    int gameId = atoi(req->message); 
    res->code = fetchDeleteReplay(gameId);
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}


bool handleShowReplay(int clientfd, Request *req, Response *res)
{
    char username[MAX_LENGTH];
    strcpy(username, strtok(req->message, "\0"));
    ReplayData replayDataArray[100];
    int numReplays = 0;
    res->code = fetchReplayDataForPlayer(replayDataArray, &numReplays, username);

    res->code = GET_USERNAME_REPLAYS_CONTINUE;
    setMessageResponse(res);
    for (int index = 0; index < numReplays; index++) {
        snprintf(res->data, sizeof(char) * MAX_LENGTH, "%d%c%s%c%s%c%s%c", replayDataArray[index].id , '@', replayDataArray[index].player1, '@', replayDataArray[index].player2, '@', replayDataArray[index].result, '\0');
        sendRes(clientfd, res, sizeof(Response), 0);
    }

    res->code = GET_USERNAME_REPLAYS_SUCCESS;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}

bool handleGetUsers(int clientfd, Request *req, Response *res) {
    printf("Message: %s\n", req->message);
    User *temp = getUsers();
    
    res->code = GET_USERS_CONTINUE;
    setMessageResponse(res);
    while (temp != NULL) {
        snprintf(res->data, sizeof(char) * MAX_LENGTH, "%s%c%s%c%s%c%d%c%d%c%d%c", temp->username, '@', temp->password, '@', temp->role, '@', temp->wins, '@', temp->losses, '@', temp->draws, '\0');
        sendRes(clientfd, res, sizeof(Response), 0);
        temp = temp->next;
    }

    res->code = GET_USERS_SUCCESS;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}

bool handleAdminAddUser(int clientfd, Request *req, Response *res) {
    printf("Message: %s\n", req->message);
    char *username, *password, *role;
    unsigned int wins, losses, draws;
    username = strtok(req->message, "@");
    password = strtok(NULL, "@");
    role = strtok(NULL, "@");
    wins = atoi(strtok(NULL, "@"));
    losses = atoi(strtok(NULL, "@"));
    draws = atoi(strtok(NULL, "\0"));
    
    res->code = adminAddUser(username, password, role, wins, losses, draws);
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}

bool handleAdminEditUser(int clientfd, Request *req, Response *res) {
    printf("Message: %s\n", req->message);
    char *username, *password, *role;
    unsigned int wins, losses, draws;
    username = strtok(req->message, "@");
    password = strtok(NULL, "@");
    role = strtok(NULL, "@");
    wins = atoi(strtok(NULL, "@"));
    losses = atoi(strtok(NULL, "@"));
    draws = atoi(strtok(NULL, "\0"));
    
    res->code = adminEditUser(username, password, role, wins, losses, draws);
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}

bool handleAdminDeleteUser(int clientfd, Request *req, Response *res) {
    printf("Message: %s\n", req->message);
    char *username;
    username = strtok(req->message, "\0");
    
    res->code = adminDeleteUser(username);
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}

bool handleWatchReplay(int clientfd, Request *req, Response *res) {
    printf("Message: %s\n", req->message);
    unsigned int replay_id;
    replay_id = atoi(strtok(req->message, "\0"));
    char moves[MAX_LENGTH];
    moves[0] = '\0';
    
    res->code = watchReplay(replay_id, moves);
    setMessageResponse(res);
    strcpy(res->data, moves);
    printf("%s\n", res->data);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}

bool handleShowAllReplayData(int clientfd, Request *req, Response *res)
{
    ReplayData replayDataArray[100];
    int numReplays = 0;
    res->code = fetchReplayDataForAllPlayers(replayDataArray, &numReplays);

    res->code = GET_ALL_REPLAYS_CONTINUE;
    setMessageResponse(res);
    for (int index = 0; index < numReplays; index++) {
        snprintf(res->data, sizeof(char) * MAX_LENGTH, "%d%c%s%c%s%c%s%c", replayDataArray[index].id , '@', replayDataArray[index].player1, '@', replayDataArray[index].player2, '@', replayDataArray[index].result, '\0');
        sendRes(clientfd, res, sizeof(Response), 0);
    }

    res->code = GET_ALL_REPLAYS_SUCCESS;
    setMessageResponse(res);
    sendRes(clientfd, res, sizeof(Response), 0);
    return true;
}