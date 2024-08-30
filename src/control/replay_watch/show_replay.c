// replay.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"



// Hàm lưu lịch sử trận đấu vào file
void saveMatchHistoryToFile(MatchHistory *history, const char *filename) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Không thể mở file %s để ghi.\n", filename);
        return;
    }
    while (history != NULL) {
        fprintf(file, "Player1: %s, Player2: %s, Game ID: %d, Result: %s\n", history->player1_name, history->player2_name, history->game_id, history->result);
        // Lưu các nước đi
        for (int i = 0; i < BOARD_LENGTH * BOARD_LENGTH * 2; i++) {
            fprintf(file, "%d ", history->moves[i]);
        }
        fprintf(file, "\n");
        history = history->next;
    }
    fclose(file);
}

// Hàm load lịch sử trận đấu từ file
MatchHistory *loadMatchHistoryFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Không thể mở file %s để đọc.\n", filename);
        return NULL;
    }

    MatchHistory *head = NULL, *tail = NULL;
    char player1_name[MAX_LENGTH];
    char player2_name[MAX_LENGTH];
    unsigned int game_id;
    char result[MAX_LENGTH];
    unsigned char moves[BOARD_LENGTH * BOARD_LENGTH * 2];

    while (fscanf(file, "Player1: %s, Player2: %s, Game ID: %u, Result: %s\n", player1_name, player2_name, &game_id, result) == 4) {
        for (int i = 0; i < BOARD_LENGTH * BOARD_LENGTH * 2; i++) {
            fscanf(file, "%hhu", &moves[i]);
        }

        MatchHistory *newNode = (MatchHistory *)malloc(sizeof(MatchHistory));
        strcpy(newNode->player1_name, player1_name);
        strcpy(newNode->player2_name, player2_name);
        newNode->game_id = game_id;
        strcpy(newNode->result, result);
        memcpy(newNode->moves, moves, sizeof(moves));
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    fclose(file);
    return head;
}

// Hàm in lịch sử trận đấu
void printMatchHistory(MatchHistory *history) {
    while (history != NULL) {
        printf("Player1: %s, Player2: %s, Game ID: %d, Result: %s\n", history->player1_name, history->player2_name, history->game_id, history->result);
        printf("Moves: ");
        for (int i = 0; i < BOARD_LENGTH * BOARD_LENGTH * 2; i++) {
            printf("%d ", history->moves[i]);
        }
        printf("\n");
        history = history->next;
    }
}

// Hàm lấy dữ liệu lịch sử đấu cho hiển thị
RES_OPCODE fetchReplayDataForDisplay(MatchHistory *history, ReplayData *replayDataArray, int *numReplays) {
    int index = 0;
    while (history != NULL && index < MAX_REPLAYS) {
        replayDataArray[index].id = history->game_id;
        strncpy(replayDataArray[index].player1, history->player1_name, sizeof(replayDataArray[index].player1) - 1);
        strncpy(replayDataArray[index].player2, history->player2_name, sizeof(replayDataArray[index].player2) - 1);
        strncpy(replayDataArray[index].result, history->result, sizeof(replayDataArray[index].result) - 1);
        index++;
        history = history->next;
    }
    *numReplays = index;

    return GET_REPLAY_SUCCESS;
}
