// replay.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"
#include "users.h"


// Hàm lấy lịch sử trận đấu của một người chơi cụ thể
RES_OPCODE fetchReplayDataForPlayer(MatchHistory *history, ReplayData *replayDataArray, int *numReplays, const char *playerName) {
    int index = 0;

    while (history != NULL && index < MAX_REPLAYS) {
        // Kiểm tra nếu người chơi cụ thể là một trong hai người chơi trong trận đấu
        if (strcmp(history->player1_name, playerName) == 0 || strcmp(history->player2_name, playerName) == 0) {
            // Sao chép dữ liệu trận đấu vào mảng replayDataArray
            replayDataArray[index].id = history->game_id;

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

    return GET_REPLAY_SUCCESS;
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

    return GET_REPLAY_SUCCESS;
}


//// Hàm lưu lịch sử trận đấu vào file
//void saveMatchHistoryToIniFile(MatchHistory* history, const char* filename) {
//    
//    static int gameCount = 1;
//
//    FILE* file = fopen("Replay.ini", "a");
//    if (file == NULL) {
//        printf("Không thể mở file %s để ghi.\n", filename);
//        return;
//    }
//    
//    while (history != NULL) {
//        fprintf(file, "[Game_%d]\n", gameCount);
//        fprintf(file, "Player1=%s\n", history->player1_name);
//        fprintf(file, "Player2=%s\n", history->player2_name);
//        fprintf(file, "GameID=%u\n", history->game_id);
//        fprintf(file, "Result=%s\n", history->result);
//        fprintf(file, "Moves=");
//        for (int i = 0; i < BOARD_LENGTH * BOARD_LENGTH * 2; i++) {
//            fprintf(file, "%d", history->moves[i]);
//            if (i < BOARD_LENGTH * BOARD_LENGTH * 2 - 1) {
//                fprintf(file, ",");
//            }
//        }
//        fprintf(file, "\n\n");
//        history = history->next;
//        gameCount++;
//    }
//    fclose(file);
//}
//
//
//// Hàm load lịch sử trận đấu từ file
//MatchHistory* loadMatchHistoryFromFile(const char* filename) {
//    FILE* file = fopen(filename, "r");
//    if (file == NULL) {
//        printf("Không thể mở file %s để đọc.\n", filename);
//        return NULL;
//    }
//
//    MatchHistory* head = NULL, * tail = NULL;
//    char line[MAX_LENGTH * 2]; // Buffer để đọc các dòng
//    char player1_name[MAX_LENGTH];
//    char player2_name[MAX_LENGTH];
//    unsigned int game_id;
//    char result[MAX_LENGTH];
//    unsigned char moves[BOARD_LENGTH * BOARD_LENGTH * 2];
//
//    while (fgets(line, sizeof(line), file)) {
//        if (strncmp(line, "[Game_", 6) == 0) {
//            // Đọc thông tin người chơi và ID trận đấu
//            fgets(line, sizeof(line), file); // Player1
//            sscanf(line, "Player1=%s\n", player1_name);
//
//            fgets(line, sizeof(line), file); // Player2
//            sscanf(line, "Player2=%s\n", player2_name);
//
//            fgets(line, sizeof(line), file); // GameID
//            sscanf(line, "GameID=%u\n", &game_id);
//
//            fgets(line, sizeof(line), file); // Result
//            sscanf(line, "Result=%s\n", result);
//
//            fgets(line, sizeof(line), file); // Moves
//            char* movesStr = strchr(line, '=') + 1; // Lấy phần sau dấu '='
//            char* move = strtok(movesStr, ",");
//            int moveIndex = 0;
//            while (move != NULL && moveIndex < BOARD_LENGTH * BOARD_LENGTH * 2) {
//                moves[moveIndex++] = (unsigned char)atoi(move);
//                move = strtok(NULL, ",");
//            }
//
//            // Tạo node mới và thêm vào danh sách liên kết
//            MatchHistory* newNode = (MatchHistory*)malloc(sizeof(MatchHistory));
//            strcpy(newNode->player1_name, player1_name);
//            strcpy(newNode->player2_name, player2_name);
//            newNode->game_id = game_id;
//            strcpy(newNode->result, result);
//            memcpy(newNode->moves, moves, sizeof(moves));
//            newNode->next = NULL;
//
//            if (head == NULL) {
//                head = newNode;
//                tail = newNode;
//            }
//            else {
//                tail->next = newNode;
//                tail = newNode;
//            }
//        }
//    }
//    fclose(file);
//    return head;
//}
//
//// Hàm in lịch sử trận đấu
//void printMatchHistory(MatchHistory *history) {
//    while (history != NULL) {
//        printf("Player1: %s, Player2: %s, Game ID: %d, Result: %s\n", history->player1_name, history->player2_name, history->game_id, history->result);
//        printf("Moves: ");
//        for (int i = 0; i < BOARD_LENGTH * BOARD_LENGTH * 2; i++) {
//            printf("%d ", history->moves[i]);
//        }
//        printf("\n");
//        history = history->next;
//    }
//}


