#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"
#include "users.h"

#define MAX_GAMES 100

Game *global_games = NULL;  // Danh sách liên kết của các trận đấu
static unsigned int gameCount = 0;  // Đếm số lượng trận đấu hiện có

// Khởi tạo danh sách các trận đấu
int initializeGame() {
    return 1;
}

int MAX_REPLAYS = 0;

// Thêm một trận đấu mới vào danh sách
int addGame(char *player1_name, char *player2_name) {
    if (gameCount >= MAX_GAMES) {
        printf("Không thể thêm trận đấu mới, danh sách đã đầy.\n");
        return 0;
    }

    Game *newGame = (Game *)malloc(sizeof(Game));
    if (newGame == NULL) {
        printf("Không thể cấp phát bộ nhớ cho trận đấu mới.\n");
        return 0;
    }

    newGame->id = gameCount + 1;  // ID cho trận đấu mới
    strncpy(newGame->player1_name, player1_name, MAX_LENGTH);
    strncpy(newGame->player2_name, player2_name, MAX_LENGTH);
    newGame->status = NOT_PLAY;  // Trạng thái ban đầu
    strncpy(newGame->result, "", MAX_LENGTH);
    newGame->moves = NULL;
    newGame->next = global_games;  // Thêm vào đầu danh sách
    global_games = newGame;
    gameCount++;
    MAX_REPLAYS = gameCount;
    return 1; // Trả về mã thành công nếu thêm thành công
}

// Thay đổi thông tin của một trận đấu
int changeGame(unsigned int id, char *player1_name, char *player2_name, GAME_STATUS status) {
    Game *current = global_games;
    while (current != NULL) {
        if (current->id == id) {
            if (player1_name != "\0") strncpy(current->player1_name, player1_name, MAX_LENGTH);
            if (player2_name != "\0")  strncpy(current->player2_name, player2_name, MAX_LENGTH);
            current->status = status;
            return 1;
        }
        current = current->next;
    }
    return 0;
}


// Thêm nước đi vào trận đấu
int addMove(unsigned int id, unsigned char x, unsigned char y) {
    Game *current = global_games;
    while (current != NULL) {
        if (current->id == id) {
            current->status = current->status == PLAYER1 ? PLAYER2 : PLAYER1;
            if (current->moves == NULL) {
                current->moves = (Move*)malloc(sizeof(Move));
                current->moves->x = x;
                current->moves->y = y;
                current->moves->next = NULL;
                return 1;
            }
            Move *curMove = current->moves;
            while (curMove->next != NULL) curMove = curMove->next;
            curMove = (Move*)malloc(sizeof(Move));
            curMove->x = x;
            curMove->y = y;
            curMove->next = NULL;
            return 1;
        }
        current = current->next;
    }
    return 0;
}


// Hoàn tác một nước đi trong trận đấu (redo)
int redoMove(Game *current) {
    if (current->moves == NULL) return 0;
    Move *curMove = current->moves;
    while (curMove->next != NULL) curMove = curMove->next;
    free(curMove);
    current->status = current->status == PLAYER1 ? PLAYER2 : PLAYER1;
    return 1;
}


// Xóa một trận đấu khỏi danh sách
int deleteGame(unsigned int id) {
    Game *current = global_games;
    Game *prev = NULL;
    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                global_games = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            gameCount--;
            MAX_REPLAYS = gameCount;
            return 0;
        }
        prev = current;
        current = current->next;
    }
    return 1;
}


/// Lấy danh sách các trận đấu của một người chơi dựa trên tên người chơi
int getGamesByUsername(char *username) {
    Game *current = global_games;
    while (current != NULL) {
        if (strcmp(current->player1_name, username) == 0 || strcmp(current->player2_name, username) == 0) {
            // In thông tin trận đấu hoặc xử lý theo yêu cầu
        }
        current = current->next;
    }
    return 1; // Trả về mã thành công
}


// Lấy danh sách tất cả các trận đấu
Game *getGames() {
    return global_games;
}

void freeGames() {
    while (global_games != NULL) {
        Game *temp = global_games;
        global_games = global_games->next;
        freeMoves(temp);
        free(temp);
    }
}

void freeMoves(Game *game) {
    while (game->moves != NULL) {
        Move *temp = game->moves;
        game->moves = game->moves->next;
        free(temp);
    }
}

// Hàm tính tỷ lệ thắng dựa trên thông tin của người chơi
double calculateWinRate(User *user) {
    int totalGames = user->wins + user->losses + user->draws;
    if (totalGames == 0) {
        return 0.0; // Tránh chia cho 0 nếu chưa có trận đấu nào
    }
    return (double)user->wins / totalGames;
}