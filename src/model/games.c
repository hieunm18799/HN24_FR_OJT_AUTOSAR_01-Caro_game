#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"
#include "users.h"

#define MAX_GAMES 100

MatchHistory* global_replay = NULL; // Khai báo và khởi tạo biến toàn cục
Game *global_games = NULL;  // Danh sách liên kết của các trận đấu
static unsigned int gameCount = 0;  // Đếm số lượng trận đấu hiện có

// Khởi tạo danh sách các trận đấu
int initializeGame() {
    return 1;
}

int MAX_REPLAYS = 0;

static Move *createMoves(unsigned char x, unsigned char y) {
    Move *ret = (Move*)malloc(sizeof(Move));
    if (ret == NULL) return NULL;
    ret->x = x;
    ret->y = y;
    ret->next = NULL;
    return ret;
}
// Thêm một trận đấu mới vào danh sách
unsigned int addGame(char *player1_name, char *player2_name) {
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
    strcpy(newGame->player1_name, player1_name);
    strcpy(newGame->player2_name, player2_name);
    strcpy(newGame->result, "");
    newGame->status = NOT_PLAY;  // Trạng thái ban đầu
    newGame->board_height = BOARD_LENGTH;
    newGame->board_width = BOARD_LENGTH;
    newGame->moves = NULL;
    newGame->next = global_games;  // Thêm vào đầu danh sách
    global_games = newGame;
    gameCount++;
    MAX_REPLAYS = gameCount;
    
    return newGame->id; // Trả về mã thành công nếu thêm thành công
}

// Thay đổi thông tin của một trận đấu
int changeGame(Game *current, char *player1_name, char *player2_name, char *result, GAME_STATUS status) {
    if (player1_name != "\0") strncpy(current->player1_name, player1_name, MAX_LENGTH);
    if (player2_name != "\0")  strncpy(current->player2_name, player2_name, MAX_LENGTH);
    if (result != "\0")  strncpy(current->result, result, MAX_LENGTH);
    current->status = status;
    return 1;
}


// Thêm nước đi vào trận đấu
int addMove(Game *current, unsigned char x, unsigned char y) {
    if (current->moves == NULL) {
        current->moves = createMoves(x, y);
        return 1;
    }
    Move *curMove = current->moves;
    while (curMove->next != NULL) curMove = curMove->next;
    curMove->next = createMoves(x,y);
    return 1;
}

Game* findGameById(unsigned int game_id) {
    Game *current = global_games;
    while (current != NULL) {
        if (current->id == game_id) return current;
        current = current->next;
    }
    return NULL;
}

// Hoàn tác một nước đi trong trận đấu (redo)
int redoMove(Game *current, unsigned char *x, unsigned char *y) {
    if (current->moves == NULL) return 0;
    
    if (current->moves->next == NULL) {
        *x = current->moves->x;
        *y = current->moves->y;
        // List has only one element
        free(current->moves);
        current->moves = NULL;
        return 1;
    }

    // Traverse the list to find the second-to-last node
    Move *temp = current->moves;
    while (temp->next->next != NULL) temp = temp->next;

    // Free the last node
    *x = temp->next->x;
    *y = temp->next->y;
    free(temp->next);
    temp->next = NULL;

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


// Hàm thêm một replay vào danh sách
void addReplay(MatchHistory** head, const char* player1_name, const char* player2_name, unsigned int game_id, const char* result, Move* moves) {
    MatchHistory* newNode = (MatchHistory*)malloc(sizeof(MatchHistory));
    if (newNode == NULL) {
        printf("Không thể cấp phát bộ nhớ cho trận đấu mới.\n");
        return;
    }

    strcpy(newNode->player1_name, player1_name);
    strcpy(newNode->player2_name, player2_name);
    newNode->game_id = game_id;
    strcpy(newNode->result, result);
    newNode->moves = moves; // Gán danh sách các nước đi
    newNode->next = *head;
    *head = newNode;
}


//ham xoa 1 replay
int deleteReplay(MatchHistory** head, unsigned int game_id) {
    MatchHistory* current = *head;
    MatchHistory* prev = NULL;

    while (current != NULL) {
        if (current->game_id == game_id) {
            if (prev == NULL) {
                *head = current->next;
            }
            else {
                prev->next = current->next;
            }

            // Giải phóng bộ nhớ của danh sách moves
            Move* tempMove;
            while (current->moves != NULL) {
                tempMove = current->moves;
                current->moves = current->moves->next;
                free(tempMove);
            }

            free(current);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}
