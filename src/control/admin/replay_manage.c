#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h" // Để truy cập vào cấu trúc và danh sách các trận đấu
#include "protocol.h"

RES_OPCODE fetchDeleteReplay(MatchHistory* head, unsigned int game_id) {
    // Kiểm tra nếu danh sách replay rỗng
    if (head == NULL) {
        return DELETE_REPLAY_NOT_FOUND;  // Không tìm thấy replay
    }

    // Gọi hàm deleteReplay để xóa replay dựa trên game_id
    int result = deleteReplay(head, game_id);

    if (result == 1) {
        return DELETE_REPLAY_SUCCESS;  // Xóa replay thành công

    }
    else {
        return DELETE_REPLAY_NOT_FOUND;  // Không tìm thấy replay để xóa

    }
}
