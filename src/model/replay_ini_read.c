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
    char line[MAX_LENGTH * 2]; // Buffer để đọc các dòng
    char player1_name[MAX_LENGTH];
    char player2_name[MAX_LENGTH];
    unsigned int game_id;
    char result[MAX_LENGTH];
    unsigned char moves[BOARD_LENGTH * BOARD_LENGTH * 2];

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "[Game_", 6) == 0) {
            // Đọc thông tin người chơi và ID trận đấu
            fgets(line, sizeof(line), file); // Player1
            sscanf(line, "Player1=%s\n", player1_name);

            fgets(line, sizeof(line), file); // Player2
            sscanf(line, "Player2=%s\n", player2_name);

            fgets(line, sizeof(line), file); // GameID
            sscanf(line, "GameID=%u\n", &game_id);

            fgets(line, sizeof(line), file); // Result
            sscanf(line, "Result=%s\n", result);

            fgets(line, sizeof(line), file); // Moves
            char* movesStr = strchr(line, '=') + 1; // Lấy phần sau dấu '='
            char* move = strtok(movesStr, ",");
            int moveIndex = 0;
            while (move != NULL && moveIndex < BOARD_LENGTH * BOARD_LENGTH * 2) {
                moves[moveIndex++] = (unsigned char)atoi(move);
                move = strtok(NULL, ",");
            }

            // Tạo node mới và thêm vào danh sách liên kết
            MatchHistory* newNode = (MatchHistory*)malloc(sizeof(MatchHistory));
            strcpy(newNode->player1_name, player1_name);
            strcpy(newNode->player2_name, player2_name);
            newNode->game_id = game_id;
            strcpy(newNode->result, result);
            memcpy(newNode->moves, moves, sizeof(moves));
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
    }
    fclose(file);
    return head;
}