#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h" // Để truy cập vào cấu trúc và danh sách các trận đấu

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
int deleteMatchById(unsigned int id) {
    Game *current = getGames(); // Lấy con trỏ đầu của danh sách các trận đấu
    Game *previous = NULL;

    // Duyệt qua danh sách để tìm trận đấu có ID khớp
    while (current != NULL) {
        if (current->id == id) {
            // Nếu tìm thấy trận đấu cần xóa
            if (previous == NULL) {
                // Nếu trận đấu cần xóa là trận đầu tiên trong danh sách
                // setGames(current->next); // Giả sử setGames() để cập nhật con trỏ đầu của danh sách
            } else {
                // Nếu không phải trận đầu tiên
                previous->next = current->next; // Bỏ qua trận đấu cần xóa
            }
            free(current); // Giải phóng bộ nhớ cho trận đấu bị xóa
            printf("Deleted game with ID: %u\n", id);
            return 1; // Trả về 1 để biểu thị thành công
        }
        previous = current; // Di chuyển previous đến current
        current = current->next; // Di chuyển đến trận đấu tiếp theo
    }

    // Nếu không tìm thấy trận đấu cần xóa
    printf("Game with ID: %u not found.\n", id);
    return 0; // Trả về 0 để biểu thị không thành công
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
