#include "games.h"
#include "users.h"

#define THRESHOLD 0.1

// Tìm kiếm người chơi khác có cùng trình độ với người đang tìm trận
RES_OPCODE findGame(User *curUser, unsigned int *game_id, User *opositePlayer) {
    double targetWinRate = calculateWinRate(curUser), smallestDifference = THRESHOLD;
    int matchingPlayers = 0;

    Game *bestGame = NULL;
    Game *current = global_games;

    // Tìm kiếm người chơi có cùng trình độ với người đang tìm trận
    while (current != NULL) {
        if (current->status == NOT_PLAY) {
            current = current->next;
            User *tempUser = findUserByName(current->player1_name);
            double currentWinRate = calculateWinRate(tempUser);
            double difference = fabs(currentWinRate - targetWinRate);
            if (difference < smallestDifference) {
                smallestDifference = difference;
                bestGame = current;
                *opositePlayer = *tempUser;
            }
        }
        current = current->next;
    }

    if (smallestDifference == THRESHOLD) {
        printf("Không tìm thấy người chơi có trình độ tương đương, đang chờ người chơi khác...\n");
        addGame(curUser->username, "", "");
        return WAITING_PLAYER; // Trả về mã đợi nếu không tìm thấy người chơi
    }

    // Cập nhật trạng thái của người chơi
    // strcpy(curUser->status, "playing");
    // strcpy(bestMatch->status, "playing");
    // printf("Tìm thấy người chơi có trình độ tương đương: %s\n", bestMatch->username);
    *game_id = bestGame->id;
    return GAME_START; // Trả về mã thành công nếu tìm thấy
}