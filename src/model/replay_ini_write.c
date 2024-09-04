#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"



// Hàm luu lich su tran dau vào file
void saveMatchHistoryToIniFile(MatchHistory* history, const char* filename) {
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        printf("Không thể mở file %s để ghi.\n", filename);
        return;
    }
    int gameCount = 1;
    while (history != NULL) {
        fprintf(file, "[Game_%d]\n", gameCount);
        fprintf(file, "Player1=%s\n", history->player1_name);
        fprintf(file, "Player2=%s\n", history->player2_name);
        fprintf(file, "GameID=%u\n", history->game_id);
        fprintf(file, "Result=%s\n", history->result);
        fprintf(file, "Moves=");

        // Lưu các nước đi
        Move* move = history->moves;
        while (move != NULL) {
            fprintf(file, "(%d,%d)", move->x, move->y);
            if (move->next != NULL) {
                fprintf(file, ",");
            }
            move = move->next;
        }
        fprintf(file, "\n\n");

        history = history->next;
        gameCount++;
    }
    fclose(file);
}
