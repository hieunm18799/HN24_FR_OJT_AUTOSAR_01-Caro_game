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
        printf("Need to wait for game/match player!\n");
        *game_id = addGame(curUser->username, "");
        return WAITING_PLAYER; // Trả về mã đợi nếu không tìm thấy người chơi
    }

    *game_id = bestGame->id;
    changeGame(bestGame->id, opositePlayer->username, curUser->username, PLAYER1);
    return GAME_START; // Trả về mã thành công nếu tìm thấy
}