#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"


// Hàm load lịch sử trận đấu từ file
MatchHistory* loadMatchHistoryFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Không thể mở file %s để đọc.\n", filename);
        return NULL;
    }

    MatchHistory* head = NULL, * tail = NULL;
    char player1_name[MAX_LENGTH];
    char player2_name[MAX_LENGTH];
    unsigned int game_id;
    char result[MAX_LENGTH];
    Move* moves = NULL, * lastMove = NULL;

    // Duyệt qua file và đọc từng dòng (giả sử format đúng)
    while (fscanf(file, "Player1=%s\nPlayer2=%s\nGameID=%u\nResult=%s\nMoves=", player1_name, player2_name, &game_id, result) == 4) {
        // Đọc các nước đi
        int x, y;
        while (fscanf(file, "(%d,%d)", &x, &y) == 2) {
            Move* newMove = (Move*)malloc(sizeof(Move));
            newMove->x = x;
            newMove->y = y;
            newMove->next = NULL;
            if (moves == NULL) {
                moves = newMove;
            }
            else {
                lastMove->next = newMove;
            }
            lastMove = newMove;
            if (fgetc(file) == '\n') break; // Ngắt khi gặp dòng mới
        }

        MatchHistory* newNode = (MatchHistory*)malloc(sizeof(MatchHistory));
        if (newNode == NULL) {
            printf("Không thể tạo node moi.\n");
        }
        strcpy(newNode->player1_name, player1_name);
        strcpy(newNode->player2_name, player2_name);
        newNode->game_id = game_id;
        strcpy(newNode->result, result);
        newNode->moves = moves;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }

        moves = NULL;
    }
    fclose(file);
    return head;
}