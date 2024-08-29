#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"
#include <math.h>  // Để sử dụng hàm fabs()
#include "users.h"

#define MAX_GAMES 100

static Game *games = NULL;  // Danh sách liên kết của các trận đấu
static unsigned int gameCount = 0;  // Đếm số lượng trận đấu hiện có

// Khởi tạo danh sách các trận đấu
int initializeGame() {
    games = NULL;
    gameCount = 0;
    return 0; // Trả về 0 nếu khởi tạo thành công
}

// Thêm một trận đấu mới vào danh sách
int addGame(char *player1_name, char *player2_name, char *result, unsigned char *moves) {
    if (gameCount >= MAX_GAMES) {
        printf("Không thể thêm trận đấu mới, danh sách đã đầy.\n");
        return -1;
    }

    Game *newGame = (Game *)malloc(sizeof(Game));
    if (newGame == NULL) {
        printf("Không thể cấp phát bộ nhớ cho trận đấu mới.\n");
        return -1;
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
    
    return 0; // Trả về 0 nếu thêm thành công
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
    current = userList;

    // Tìm kiếm người chơi có cùng trình độ với người đang tìm trận
    while (current != NULL) {
        if (current == targetUser || strcmp(current->status, "playing") == 0) {
            current = current->next;
            continue;
        }

        double currentWinRate = calculateWinRate(current);
        if (abs(currentWinRate - targetWinRate) < 0.1) { // Tìm người chơi có tỷ lệ thắng gần bằng
            bestMatch = current;
            break;
        }
        current = current->next;
    }

    if (bestMatch != NULL) {
        *matchedUser = bestMatch; // Gán người chơi tìm thấy vào con trỏ
        printf("Tìm thấy người chơi có trình độ tương đương: %s\n", bestMatch->username);
        return GAME_FOUND; // Trả về mã thành công nếu tìm thấy
    } else {
        printf("Không tìm thấy người chơi có trình độ tương đương.\n");
        return GAME_NOT_FOUND; // Không tìm thấy, trả về mã lỗi
    }
}

// Thay đổi thông tin của một trận đấu
int changeGame(unsigned int id, char *player1_name, char *player2_name) {
    Game *current = games;
    while (current != NULL) {
        if (current->id == id) {
            strncpy(current->player1_name, player1_name, MAX_LENGTH);
            strncpy(current->player2_name, player2_name, MAX_LENGTH);
            return 0; // Trả về 0 nếu thay đổi thành công
        }
        current = current->next;
    }
    return -1; // Trả về -1 nếu không tìm thấy trận đấu
}

// Thêm nước đi vào trận đấu
int addMove(unsigned int id, char *player_name, unsigned char x, unsigned char y) {
    Game *current = games;
    while (current != NULL) {
        if (current->id == id) {
            // Thêm nước đi của người chơi vào danh sách nước đi
            size_t moveIndex = current->status == PLAYER1 ? 0 : 1;
            moveIndex *= BOARD_LENGTH * BOARD_LENGTH;
            moveIndex += x * BOARD_LENGTH + y;
              current->moves[moveIndex] = 1; // Đánh dấu nước đi
            current->status = current->status == PLAYER1 ? PLAYER2 : PLAYER1; // Chuyển lượt người chơi
            return 0; // Trả về 0 nếu thêm nước đi thành công
        }
        current = current->next;
    }
    return -1; // Trả về -1 nếu không tìm thấy trận đấu
}

// Hoàn tác một nước đi trong trận đấu (redo)
int changeMove(unsigned int id, unsigned char x, unsigned char y) {
    Game *current = games;
    while (current != NULL) {
        if (current->id == id) {
            size_t moveIndex = x * BOARD_LENGTH + y;
            if (current->moves[moveIndex] == 1) {
                current->moves[moveIndex] = 0; // Bỏ nước đi
                return 0; // Trả về 0 nếu thay đổi nước đi thành công
            }
        }
        current = current->next;
    }
    return -1; // Trả về -1 nếu không tìm thấy trận đấu
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
            return 0; // Trả về 0 nếu xóa thành công
        }
        prev = current;
        current = current->next;
    }
    return -1; // Trả về -1 nếu không tìm thấy trận đấu
}

// Lấy danh sách các trận đấu của một người chơi dựa trên tên người chơi
int getGamesByUsername(char *username) {
    Game *current = games;
    while (current != NULL) {
        if (strcmp(current->player1_name, username) == 0 || strcmp(current->player2_name, username) == 0) {
            // In thông tin trận đấu hoặc xử lý theo yêu cầu
        }
        current = current->next;
    }
    return 0; // Trả về 0 nếu thành công
}

// Lấy danh sách tất cả các trận đấu
Game *getGames() {
    return games;
}

