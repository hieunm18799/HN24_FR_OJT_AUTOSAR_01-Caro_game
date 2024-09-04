#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"



// Hàm luu lich su tran dau vào file
void saveMatchHistoryToIniFile(MatchHistory* global_replay, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Không thể mở file %s để ghi.\n", filename);
        return;
    }
    int gameCount = 1;
    while (global_replay != NULL) {
        fprintf(file, "[Game_%d]\n", gameCount);
        fprintf(file, "Player1=%s\n", global_replay->player1_name);
        fprintf(file, "Player2=%s\n", global_replay->player2_name);
        fprintf(file, "GameID=%u\n", global_replay->game_id);
        fprintf(file, "Result=%s\n", global_replay->result);
        fprintf(file, "Moves=");

        // Lưu các nước đi
        Move* move = global_replay->moves;
        while (move != NULL) {
            fprintf(file, "(%d,%d)", move->x, move->y);
            if (move->next != NULL) {
                fprintf(file, ",");
            }
            move = move->next;
        }
        fprintf(file, "\n\n");

        global_replay = global_replay->next;
        gameCount++;
    }
    fclose(file);
}
