// replay.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"
#include "users.h"


// Hàm lấy lịch sử trận đấu của một người chơi cụ thể
RES_OPCODE fetchReplayDataForPlayer(ReplayData *replayDataArray, int *numReplays, const char *playerName) {
    int index = 0;
    MatchHistory *history = loadMatchHistoryFromFile("Re_play.ini");
    
    while (history != NULL && index < MAX_REPLAYS) {
        // Kiểm tra nếu người chơi cụ thể là một trong hai người chơi trong trận đấu
        if (strcmp(history->player1_name, playerName) == 0 || strcmp(history->player2_name, playerName) == 0) {
            // Sao chép dữ liệu trận đấu vào mảng replayDataArray
            replayDataArray[index].id = history->game_id;
            printf("%d\n", replayDataArray[index].id);

            strncpy(replayDataArray[index].player1, history->player1_name, sizeof(replayDataArray[index].player1) - 1);
            replayDataArray[index].player1[sizeof(replayDataArray[index].player1) - 1] = '\0';

            strncpy(replayDataArray[index].player2, history->player2_name, sizeof(replayDataArray[index].player2) - 1);
            replayDataArray[index].player2[sizeof(replayDataArray[index].player2) - 1] = '\0';

            strncpy(replayDataArray[index].result, history->result, sizeof(replayDataArray[index].result) - 1);
            replayDataArray[index].result[sizeof(replayDataArray[index].result) - 1] = '\0';

            index++;
        }
        history = history->next;
    }

    *numReplays = index;

    return GET_REPLAYS_SUCCESS;
}

RES_OPCODE fetchReplayDataForAllPlayers(MatchHistory *history, ReplayData *replayDataArray, int *numReplays) {
    int index = 0;

    while (history != NULL && index < MAX_REPLAYS) {
        // Sao chép dữ liệu trận đấu vào mảng replayDataArray
        replayDataArray[index].id = history->game_id;

        strncpy(replayDataArray[index].player1, history->player1_name, sizeof(replayDataArray[index].player1) - 1);
        replayDataArray[index].player1[sizeof(replayDataArray[index].player1) - 1] = '\0';

        strncpy(replayDataArray[index].player2, history->player2_name, sizeof(replayDataArray[index].player2) - 1);
        replayDataArray[index].player2[sizeof(replayDataArray[index].player2) - 1] = '\0';

        strncpy(replayDataArray[index].result, history->result, sizeof(replayDataArray[index].result) - 1);
        replayDataArray[index].result[sizeof(replayDataArray[index].result) - 1] = '\0';

        index++;
        history = history->next;
    }

    *numReplays = index;

    return GET_REPLAYS_SUCCESS;
}