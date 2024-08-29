// replay.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"

// định nghĩa 1 struct lịch sử đấu
typedef struct MatchHistory {
    char player_name[MAX_LENGTH];
    unsigned int game_id;
    char result[MAX_LENGTH];
    unsigned char moves[BOARD_LENGTH * BOARD_LENGTH * 2];
    struct MatchHistory *next;
} MatchHistory;




// Hàm lưu lịch sử trận đấu vào file
void saveMatchHistoryToFile(MatchHistory *history, const char *filename) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Không thể mở file %s để ghi.\n", filename);
        return;
    }
    while (history != NULL) {
        fprintf(file, "Player: %s, Game ID: %d, Result: %s\n", history->player_name, history->game_id, history->result);
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
    char player_name[MAX_LENGTH];
    unsigned int game_id;
    char result[MAX_LENGTH];
    unsigned char moves[BOARD_LENGTH * BOARD_LENGTH * 2];

    while (fscanf(file, "Player: %s, Game ID: %hu, Result: %s\n", player_name, &game_id, result) == 3) {
        for (int i = 0; i < BOARD_LENGTH * BOARD_LENGTH * 2; i++) {
            fscanf(file, "%hhu", &moves[i]);
        }

        MatchHistory *newNode = (MatchHistory *)malloc(sizeof(MatchHistory));
        strcpy(newNode->player_name, player_name);
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
        printf("Player: %s, Game ID: %d, Result: %s\n", history->player_name, history->game_id, history->result);
        printf("Moves: ");
        for (int i = 0; i < BOARD_LENGTH * BOARD_LENGTH * 2; i++) {
            printf("%d ", history->moves[i]);
        }
        printf("\n");
        history = history->next;
    }
}
