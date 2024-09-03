#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h" 


// Hàm xóa lịch sử đấu theo ID
int deleteReplay(MatchHistory** head, unsigned int game_id) {
    if (head == NULL || *head == NULL) {
        return 0; // Danh sách rỗng hoặc con trỏ head rỗng
    }

    MatchHistory* current = *head;
    MatchHistory* previous = NULL;

    // Duyệt qua danh sách để tìm trận đấu có game_id khớp
    while (current != NULL) {
        if (current->game_id == game_id) {
            // Nếu tìm thấy, cập nhật các liên kết để bỏ qua node này
            if (previous == NULL) {
                // Trường hợp node cần xóa là node đầu tiên
                *head = current->next;
            }
            else {
                // Trường hợp node cần xóa không phải là node đầu tiên
                previous->next = current->next;
            }
            free(current); // Giải phóng bộ nhớ của node bị xóa
            return 1; // Trả về 1 nếu xóa thành công
        }
        previous = current;
        current = current->next;
    }

    return 0; // Trả về 0 nếu không tìm thấy game_id
}

RES_OPCODE fetchdeleteReplay(MatchHistory** head, int game_id) {
    if (head == NULL || *head == NULL) {
        return DELETE_REPLAY_FAILURE; // Danh sách rỗng hoặc con trỏ head rỗng
    }

    MatchHistory* current = *head;
    MatchHistory* previous = NULL;

    // Duyệt qua danh sách để tìm trận đấu có game_id khớp
    while (current != NULL) {
        if (current->game_id == game_id) {
            // Nếu tìm thấy, cập nhật các liên kết để bỏ qua node này
            if (previous == NULL) {
                // Trường hợp node cần xóa là node đầu tiên
                *head = current->next;
            } else {
                // Trường hợp node cần xóa không phải là node đầu tiên
                previous->next = current->next;
            }
            free(current); // Giải phóng bộ nhớ của node bị xóa
            return DELETE_REPLAY_SUCCESS; // Trả về mã thành công nếu xóa thành công
        }
        previous = current;
        current = current->next;
    }

    return DELETE_REPLAY_FAILURE; // Trả về mã lỗi nếu không tìm thấy game_id
}
