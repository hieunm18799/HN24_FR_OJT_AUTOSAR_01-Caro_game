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
    __uint16_t id;
    char player1_name[MAX_LENGTH];
    char player2_name[MAX_LENGTH];
    GAME_STATUS status;
    char result[MAX_LENGTH];
    __uint8_t moves[BOARD_LENGTH * BOARD_LENGTH * 2];
    struct Game *next;
} Game;

int initializeGame();


int addGame(char *player1_name, char *player2_name, char *result, __uint8_t *moves);
int findGame();
int changeGame(__uint16_t id, char *player1_name, char *player2_name);
int addMove(__uint16_t id, char *player1_name, __uint8_t x, __uint8_t y);
int changeMove(__uint16_t id, __uint8_t x, __uint8_t y); //redo
int deleteGame(__uint16_t id);
int getGamesByUsername(char *username);
Game *getGames();

#endif