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
    unsigned char x, y;
    struct Move *next;
} Move;

typedef struct Game {
    unsigned int id;
    char player1_name[MAX_LENGTH], player2_name[MAX_LENGTH], result[MAX_LENGTH];
    unsigned char board_width, board_height;
    GAME_STATUS status;
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
unsigned int addGame(char *player1_name, char *player2_name);
int changeGame(Game *game, char *player1_name, char *player2_name, char *result, GAME_STATUS status);
int addMove(Game *game, unsigned char x, unsigned char y);
Game* findGameById(unsigned int game_id);
int redoMove(Game *current, unsigned char *x, unsigned char *y); //redo
int deleteGame(unsigned int id);
int getGamesByUsername(char *username);
Game *getGames();
void freeGames();
void freeMoves(Game *game);

#endif