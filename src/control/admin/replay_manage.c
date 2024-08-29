#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h" // Để truy cập vào cấu trúc và danh sách các trận đấu

// Hàm hiển thị tất cả các lịch sử đấu
void displayAllMatches() {
    Game *current = getGames(); // Giả sử getGames() trả về con trỏ đầu của danh sách các trận đấu
    while (current != NULL) {
        printf("Game ID: %d\n", current->id);
        printf("Player 1: %s\n", current->player1_name);
        printf("Player 2: %s\n", current->player2_name);
        printf("Status: %d\n", current->status);
        printf("Result: %s\n\n", current->result);
        current = current->next;
    }
}

// Hàm xóa lịch sử đấu theo ID
int deleteMatchById(__uint16_t id) {
    Game *current = getGames(); // Giả sử getGames() trả về con trỏ đầu của danh sách các trận đấu
    Game *previous = NULL;
    while (current != NULL) {
        if (current->id == id) {
            if (previous == NULL) {
                // Nếu trận đấu cần xóa là trận đầu tiên trong danh sách
                setGames(current->next); // Giả sử setGames() để cập nhật con trỏ đầu của danh sách
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("Deleted game with ID: %d\n", id);
            return 1; // Thành công
        }
        previous = current;
        current = current->next;
    }
    printf("Game with ID: %d not found.\n", id);
    return 0; // Không tìm thấy
}

void saveGamesToFile() {
    FILE *file = fopen("game_history.txt", "w");
    if (!file) {
        perror("Could not open file");
        return;
    }

    Game *current = getGames();
    while (current != NULL) {
        fprintf(file, "%d,%s,%s,%d,%s\n", current->id, current->player1_name, current->player2_name, current->status, current->result);
        current = current->next;
    }

    fclose(file);
    printf("Games saved to file successfully.\n");
}
