#ifndef GAMES_H
#define GAMES_H

#include "protocol.h"

extern int MAX_REPLAYS;

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


extern Game *global_games;


// Định nghĩa 1 struct lịch sử đấu
typedef struct MatchHistory {
    char player1_name[MAX_LENGTH];
    char player2_name[MAX_LENGTH];
    unsigned int game_id;
    char result[MAX_LENGTH];
    unsigned char moves[BOARD_LENGTH * BOARD_LENGTH * 2];
    struct MatchHistory *next;
} MatchHistory;

// Định nghĩa cấu trúc để giữ dữ liệu replay
typedef struct {
    int id;
    char player1[50];
    char player2[50];
    char result[10];
} ReplayData;

int initializeGame();
int addGame(char *player1_name, char *player2_name);
int changeGame(unsigned int id, char *player1_name, char *player2_name, GAME_STATUS status);
int addMove(unsigned int id, unsigned char x, unsigned char y);
int changeMove(unsigned int id, unsigned char x, unsigned char y); //redo
int deleteGame(unsigned int id);
int getGamesByUsername(char *username);
Game *getGames();
void freeGames();

#endif