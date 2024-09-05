#include "users.h"
#include "protocol.h"

RES_OPCODE sign_out(int clientfd, char *username) {
    if (username == NULL) return SIGN_OUT_FAIL;
    User* user = getUsers();
    while (user) {
        if (strcmp(user->username, username) == 0) {
            if (user->clientfd == clientfd) {
                // Cập nhật trạng thái người dùng
                setUserStatus(username, "NOT_SIGN_IN");
                user->clientfd = INVALID_SOCKET;

                // Phản hồi đăng xuất thành công
                return SIGN_OUT_SUCCESS;
            } else {
                return SIGN_OUT_FAIL;
            }
        }
        user = user->next;
    }

    // Nếu không tìm thấy username
    return USERNAME_NOT_EXISTED;
}
