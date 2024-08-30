#ifndef GAMES_H
#define GAMES_H

#include "protocol.h"

typedef enum GAME_STATUS {
    NOT_PLAY,
    PLAYER1,
    PLAYER2,
    END,
} GAME_STATUS;

typedef struct Move {
    unsigned char x;
    unsigned char y;
    struct Move *next;
} Move;

typedef struct Game {
    unsigned int id;
    char player1_name[MAX_LENGTH];
    char player2_name[MAX_LENGTH];
    GAME_STATUS status;
    char result[MAX_LENGTH];
    Move *moves;
    struct Game *next;
} Game;

int MAX_REPLAYS;

int initializeGame();
int addGame(char *player1_name, char *player2_name);
int changeGame(unsigned int id, char *player1_name, char *player2_name, GAME_STATUS status);
int addMove(unsigned int id, unsigned char x, unsigned char y);
int redoMove(Game *current); //redo
int deleteGame(unsigned int id);
int getGamesByUsername(char *username);
Game *getGames();
void freeGames();
void freeMoves(Game *game);

#endif