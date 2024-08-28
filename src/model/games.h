#ifndef GAMES_H
#define GAMES_H

#include "protocol.h"

typedef enum GAME_STATUS {
    NOT_PLAY,
    PLAYER1,
    PLAYER2,
    END,
} GAME_STATUS;

typedef struct Game {
    unsigned int id;
    char player1_name[MAX_LENGTH];
    char player2_name[MAX_LENGTH];
    GAME_STATUS status;
    char result[MAX_LENGTH];
    unsigned char moves[BOARD_LENGTH * BOARD_LENGTH * 2];
    struct Game *next;
} Game;

int initializeGame();


int addGame(char *player1_name, char *player2_name, char *result, unsigned char *moves);
int findGame();
int changeGame(unsigned int id, char *player1_name, char *player2_name);
int addMove(unsigned int id, char *player1_name, unsigned char x, unsigned char y);
int changeMove(unsigned int id, unsigned char x, unsigned char y); //redo
int deleteGame(unsigned int id);
int getGamesByUsername(char *username);
Game *getGames();

#endif