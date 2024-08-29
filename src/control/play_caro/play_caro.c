#include "games.h"
#include "users.h"

// Tìm kiếm người chơi khác có cùng trình độ với người đang tìm trận
RES_OPCODE pickCaro(char* username, unsigned int game_id, unsigned char x, unsigned char y, SOCKET *opofd) {
    Game *curGame = global_games;

    // Tìm kiếm người chơi có cùng trình độ với người đang tìm trận
    while (curGame != NULL) {
        if (curGame->id != game_id) curGame = curGame->next;
        else break;
    }

    if (strcmp(username, curGame->player1_name) == 0 && curGame->status == PLAYER2 || strcmp(username, curGame->player2_name) == 0 && curGame->status == PLAYER1) return OTHER_PLAYER_TURN;

    int res = addMove(game_id, x, y);
    if (res == 0) return PICK_FAIL;
    *opofd = findUserByName(strcmp(username, curGame->player1_name) == 0 ? curGame->player2_name : curGame->player1_name)->clientfd;
    return YOUR_TURN;
}