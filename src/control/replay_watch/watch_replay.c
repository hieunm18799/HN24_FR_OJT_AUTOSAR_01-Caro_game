#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"

RES_OPCODE watchReplay(unsigned int replay_id, char *moves) {
    MatchHistory *history = loadMatchHistoryFromFile("Re_play.ini");
    
    while (history != NULL) {
        // Kiểm tra nếu người chơi cụ thể là một trong hai người chơi trong trận đấu
        if (history->game_id == replay_id) {
            // Sao chép dữ liệu trận đấu vào mảng replayDataArray
            Move *temp = history->moves;
            strcpy(moves, "\0");
            while (temp != NULL) {
                char str[50];
                if (temp->next == NULL) sprintf(str, "%d%c%d%c", temp->x, '-', temp->y, '\0');
                else sprintf(str, "%d%c%d%c", temp->x, '-', temp->y, '@');
                strcat(moves, str);
                temp = temp->next;
            }
            return GET_REPLAYID_MOVES_SUCCESS;
        }
        history = history->next;
    }

    return GET_REPLAYID_MOVES_FAIL;
}
