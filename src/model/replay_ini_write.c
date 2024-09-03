#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"



// Hàm lưu lịch sử trận đấu vào file
void saveMatchHistoryToIniFile(MatchHistory* history, const char* filename) {

    static int gameCount = 1;

    FILE* file = fopen("Replay.ini", "a");
    if (file == NULL) {
        printf("Không thể mở file %s để ghi.\n", filename);
        return;
    }

    while (history != NULL) {
        fprintf(file, "[Game_%d]\n", gameCount);
        fprintf(file, "Player1=%s\n", history->player1_name);
        fprintf(file, "Player2=%s\n", history->player2_name);
        fprintf(file, "GameID=%u\n", history->game_id);
        fprintf(file, "Result=%s\n", history->result);
        fprintf(file, "Moves=");
        for (int i = 0; i < BOARD_LENGTH * BOARD_LENGTH * 2; i++) {
            fprintf(file, "%d", history->moves[i]);
            if (i < BOARD_LENGTH * BOARD_LENGTH * 2 - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n\n");
        history = history->next;
        gameCount++;
    }
    fclose(file);
}
