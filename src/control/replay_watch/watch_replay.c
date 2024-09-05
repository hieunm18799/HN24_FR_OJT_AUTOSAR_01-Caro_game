#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"

RES_OPCODE watchReplay(unsigned int replay_id, char *moves) {
    MatchHistory *history = loadMatchHistoryFromFile("Re_play.ini");
    
    while (history != NULL) {
        // Kiểm tra nếu trận đấu có ID khớp với replay_id
        if (history->game_id == replay_id) {
            // Sao chép dữ liệu trận đấu vào mảng moves
            Move *temp = history->moves;
            moves[0] = '\0';  // Khởi tạo chuỗi rỗng thay vì dùng "\0"
            
            while (temp != NULL) {
                char str[50];
                // Kiểm tra nếu nước đi là cuối cùng, không thêm ký tự '@'
                if (temp->next != NULL) {
                    sprintf(str, "%d-%d@", temp->x, temp->y);                   
                } else {
                    sprintf(str, "%d-%d@", temp->x, temp->y);   
                }
                strcat(moves, str);
                temp = temp->next;
            }
            return GET_REPLAYID_MOVES_SUCCESS;
        }
        history = history->next;
    }

    return GET_REPLAYID_MOVES_FAIL;
}

