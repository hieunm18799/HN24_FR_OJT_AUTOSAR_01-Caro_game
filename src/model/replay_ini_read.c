#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"


MatchHistory* loadMatchHistoryFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Không thể mở file %s để đọc.\n", filename);
        return NULL;
    }

    MatchHistory* head = NULL, * tail = NULL;
    char line[MAX_LENGTH];
    char player1_name[MAX_LENGTH];
    char player2_name[MAX_LENGTH];
    unsigned int game_id;
    char result[MAX_LENGTH];
    Move* moves = NULL, * lastMove = NULL;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "[Game_", 6) == 0) {
            // Bắt đầu một trận đấu mới, lưu thông tin trận đấu trước đó nếu có
            if (moves != NULL) {
                MatchHistory* newNode = (MatchHistory*)malloc(sizeof(MatchHistory));
                if (newNode == NULL) {
                    printf("Không thể tạo node mới.\n");
                    fclose(file);
                    return head; // Trả về danh sách hiện tại
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
        }
        else if (sscanf(line, "Player1=%s", player1_name) == 1) {
            // Đọc tên Player1
        }
        else if (sscanf(line, "Player2=%s", player2_name) == 1) {
            // Đọc tên Player2
        }
        else if (sscanf(line, "GameID=%u", &game_id) == 1) {
            // Đọc GameID
        }
        else if (sscanf(line, "Result=%s", result) == 1) {
            // Đọc kết quả
        }
        else if (strncmp(line, "Moves=", 6) == 0) {
            // Đọc các nước đi
            char* movesStr = line + 6;
            int x, y;
            char* move = strtok(movesStr, ",");
            while (move != NULL) {
                sscanf(move, "(%d-%d)", &x, &y);
                Move* newMove = (Move*)malloc(sizeof(Move));
                if (newMove == NULL) {
                    printf("Không thể tạo move mới.\n");
                    fclose(file);
                    return head; // Trả về danh sách hiện tại
                }
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
                move = strtok(NULL, ",");
            }
        }
    }

    // Lưu thông tin trận đấu cuối cùng
    if (moves != NULL) {
        MatchHistory* newNode = (MatchHistory*)malloc(sizeof(MatchHistory));
        if (newNode == NULL) {
            printf("Không thể tạo node mới.\n");
            fclose(file);
            return head; // Trả về danh sách hiện tại
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
    }

    fclose(file);
    return head;
}