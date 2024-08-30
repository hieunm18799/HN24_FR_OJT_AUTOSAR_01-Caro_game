#include "games.h"
#include "users.h"

RES_OPCODE pickCaro(char* username, unsigned int game_id, unsigned char x, unsigned char y, SOCKET *opofd) {
    Game *curGame = global_games;

    while (curGame != NULL) {
        if (curGame->id != game_id) curGame = curGame->next;
        else break;
    }

    if (strcmp(username, curGame->player1_name) == 0 && curGame->status == PLAYER2 || strcmp(username, curGame->player2_name) == 0 && curGame->status == PLAYER1) return OTHER_PLAYER_TURN;

    int res = addMove(game_id, x, y);
    if (res == 0) return PICK_FAIL;
    *opofd = findUserByName(strcmp(username, curGame->player1_name) == 0 ? curGame->player2_name : curGame->player1_name)->clientfd;
    return PICK_SUCCESS;
}

RES_OPCODE redoAsk(char* username, unsigned int game_id, User *opoUser) {
    Game *curGame = global_games;

    while (curGame != NULL) {
        if (curGame->id != game_id) curGame = curGame->next;
        else break;
    }

    char *opoUserName;
    strcpy(opoUserName, strcmp(curGame->player1_name, username) == 0 ? curGame->player2_name : curGame->player1_name);

    opoUser = findUserByName(opoUserName);
    return REDO_ASK;
}

RES_OPCODE redoAgree(char* username, unsigned int game_id, User *opoUser) {
    Game *curGame = global_games;

    while (curGame != NULL) {
        if (curGame->id != game_id) curGame = curGame->next;
        else break;
    }

    if ((strcmp(curGame->player1_name, username) == 0 && curGame->status == PLAYER2) || (strcmp(curGame->player2_name, username) == 0 && curGame->status == PLAYER2)) return REDO_FAIL;
    
    char *opoUserName;
    strcpy(opoUserName, strcmp(curGame->player1_name, username) == 0 ? curGame->player2_name : curGame->player1_name);
    opoUser = findUserByName(opoUserName);

    redoMove(curGame);
    return YOUR_TURN;
}

RES_OPCODE quitLogic(char* username, unsigned int game_id, User *opoUser) {
    Game *curGame = global_games;

    while (curGame != NULL) {
        if (curGame->id != game_id) curGame = curGame->next;
        else break;
    }

    if (curGame->status == NOT_PLAY || curGame->status == END) return QUIT_SUCCESS;

    char *opoUserName;
    strcpy(opoUserName, strcmp(curGame->player1_name, username) == 0 ? curGame->player2_name : curGame->player1_name);
    opoUser = findUserByName(opoUserName);

    changeGame(game_id, "\0", "\0", END);
    return OTHER_PLAYER_WIN;
}