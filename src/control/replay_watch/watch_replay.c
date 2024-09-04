#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"




void sendReplayData(MatchHistory* replay) {
    if (replay == NULL) {
        printf("Không có dữ liệu để hiển thị.\n");
        return;
    }

    // Gửi tên người chơi và ID game
    printf("Player 1: %s\n", replay->player1_name);
    printf("Player 2: %s\n", replay->player2_name);
    printf("Game ID: %u\n", replay->game_id);
    printf("Kết quả: %s\n", replay->result);

    // Hiển thị thứ tự các nước đi
    Move* currentMove = replay->moves;
    int moveNumber = 1;
    while (currentMove != NULL) {
        printf("Nước đi %d: (%d, %d)\n", moveNumber, currentMove->x, currentMove->y);
        moveNumber++;
        currentMove = currentMove->next;
    }
}