#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"
#include "users.h"

#define MAX_GAMES 100

static Game *games = NULL;  // Danh sách liên kết của các trận đấu
static unsigned int gameCount = 0;  // Đếm số lượng trận đấu hiện có

// Khởi tạo danh sách các trận đấu
int initializeGame() {
    games = NULL;
    gameCount = 0;
    return GAME_UPDATE_SUCCESS; // Trả về mã thành công nếu khởi tạo thành công
}


// Thêm một trận đấu mới vào danh sách
int addGame(char *player1_name, char *player2_name, char *result, unsigned char *moves) {
    if (gameCount >= MAX_GAMES) {
        printf("Không thể thêm trận đấu mới, danh sách đã đầy.\n");
        return GAME_ADD_FAIL;
    }

    Game *newGame = (Game *)malloc(sizeof(Game));
    if (newGame == NULL) {
        printf("Không thể cấp phát bộ nhớ cho trận đấu mới.\n");
        return GAME_ADD_FAIL;
    }

    newGame->id = gameCount + 1;  // ID cho trận đấu mới
    strncpy(newGame->player1_name, player1_name, MAX_LENGTH);
    strncpy(newGame->player2_name, player2_name, MAX_LENGTH);
    newGame->status = NOT_PLAY;  // Trạng thái ban đầu
    strncpy(newGame->result, result, MAX_LENGTH);
    memcpy(newGame->moves, moves, BOARD_LENGTH * BOARD_LENGTH * 2);
    newGame->next = games;  // Thêm vào đầu danh sách
    games = newGame;
    gameCount++;
    
    return GAME_ADD_SUCCESS; // Trả về mã thành công nếu thêm thành công
}



// Hàm tính tỷ lệ thắng dựa trên thông tin của người chơi
double calculateWinRate(User *user) {
    int totalGames = user->wins + user->losses + user->draws;
    if (totalGames == 0) {
        return 0.0; // Tránh chia cho 0 nếu chưa có trận đấu nào
    }
    return (double)user->wins / totalGames;
}

// Tìm kiếm người chơi khác có cùng trình độ với người đang tìm trận
int findGame(char *player_name, User **matchedUser) {
    User *current = userList; // `userList` là con trỏ tới danh sách người chơi
    User *targetUser = NULL;

    // Tìm người chơi đang tìm trận dựa trên tên
    while (current != NULL) {
        if (strcmp(current->username, player_name) == 0) {
            targetUser = current;
            break;
        }
        current = current->next;
    }

    if (targetUser == NULL) {
        return USERNAME_NOT_EXISTED; // Không tìm thấy người chơi
    }

    double targetWinRate = calculateWinRate(targetUser);
    User *bestMatch = NULL;
    double smallestDifference = 1;
    int matchingPlayers = 0;

    current = userList;

    // Tìm kiếm người chơi có cùng trình độ với người đang tìm trận
    while (current != NULL) {
        if (current == targetUser || strcmp(current->status, "playing") == 0 || strcmp(current->status, "waiting") == 0) {
            current = current->next;
            continue;
        }

        double currentWinRate = calculateWinRate(current);
        double difference = fabs(currentWinRate - targetWinRate);

        if (difference < 0.1) {
            matchingPlayers++;
            if (difference < smallestDifference) {
                smallestDifference = difference;
                bestMatch = current;
            }
        }
        current = current->next;
    }

    if (matchingPlayers == 0) {
        printf("Không tìm thấy người chơi có trình độ tương đương, đang chờ người chơi khác...\n");
        // Cập nhật trạng thái của người chơi hiện tại
        strcpy(targetUser->status, "waiting");
        return WAITING_PLAYER; // Trả về mã đợi nếu không tìm thấy người chơi
    }

    // Nếu tìm thấy người chơi phù hợp
    if (bestMatch != NULL) {
        *matchedUser = bestMatch; // Gán người chơi tìm thấy vào con trỏ
        // Cập nhật trạng thái của người chơi
        strcpy(targetUser->status, "playing");
        strcpy(bestMatch->status, "playing");
        printf("Tìm thấy người chơi có trình độ tương đương: %s\n", bestMatch->username);
        return GAME_FOUND; // Trả về mã thành công nếu tìm thấy
    }

    return GAME_NOT_FOUND; // Trả về mã lỗi nếu không tìm thấy trận đấu
}

// Thay đổi thông tin của một trận đấu
int changeGame(unsigned int id, char *player1_name, char *player2_name) {
    Game *current = games;
    while (current != NULL) {
        if (current->id == id) {
            strncpy(current->player1_name, player1_name, MAX_LENGTH);
            strncpy(current->player2_name, player2_name, MAX_LENGTH);
            return GAME_UPDATE_SUCCESS;
        }
        current = current->next;
    }
    return GAME_NOT_FOUND;
}


// Thêm nước đi vào trận đấu
int addMove(unsigned int id, char *player_name, unsigned char x, unsigned char y) {
    Game *current = games;
    while (current != NULL) {
        if (current->id == id) {
            size_t moveIndex = current->status == PLAYER1 ? 0 : 1;
            moveIndex *= BOARD_LENGTH * BOARD_LENGTH;
            moveIndex += x * BOARD_LENGTH + y;
            current->moves[moveIndex] = 1;
            current->status = current->status == PLAYER1 ? PLAYER2 : PLAYER1;
            return MOVE_ADD_SUCCESS;
        }
        current = current->next;
    }
    return GAME_NOT_FOUND;
}


// Hoàn tác một nước đi trong trận đấu (redo)
int changeMove(unsigned int id, unsigned char x, unsigned char y) {
    Game *current = games;
    while (current != NULL) {
        if (current->id == id) {
            size_t moveIndex = x * BOARD_LENGTH + y;
            if (current->moves[moveIndex] == 1) {
                current->moves[moveIndex] = 0;
                return MOVE_CHANGE_SUCCESS;
            }
        }
        current = current->next;
    }
    return GAME_NOT_FOUND;
}


// Xóa một trận đấu khỏi danh sách
int deleteGame(unsigned int id) {
    Game *current = games;
    Game *prev = NULL;
    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                games = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            gameCount--;
            return GAME_DELETE_SUCCESS;
        }
        prev = current;
        current = current->next;
    }
    return GAME_NOT_FOUND;
}


/// Lấy danh sách các trận đấu của một người chơi dựa trên tên người chơi
int getGamesByUsername(char *username) {
    Game *current = games;
    while (current != NULL) {
        if (strcmp(current->player1_name, username) == 0 || strcmp(current->player2_name, username) == 0) {
            // In thông tin trận đấu hoặc xử lý theo yêu cầu
        }
        current = current->next;
    }
    return GAME_UPDATE_SUCCESS; // Trả về mã thành công
}


// Lấy danh sách tất cả các trận đấu
Game *getGames() {
    return games;
}

