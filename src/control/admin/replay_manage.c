#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h" 


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

