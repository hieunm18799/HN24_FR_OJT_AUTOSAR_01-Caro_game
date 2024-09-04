#include "users.h"
#include "protocol.h"

RES_OPCODE sign_in(int clientfd, char *username, char *password, char* role) {
    if (username == NULL || password == NULL) return SIGN_IN_INPUT_WRONG;
    User* user = getUsers();
    while (user) {
        if (strcmp(user->username, username) == 0) {
            // Kiểm tra password
            if (strcmp(user->password, password) == 0) {
                if (strcmp(user->status, "NOT_SIGN_IN") == 0) {
                    // Cập nhật trạng thái người dùng
                    setUserStatus(username, "SIGNED_IN");
                    user->clientfd = clientfd;
                    strcpy(role, user->role);
                    // Phản hồi đăng nhập thành công
                    return SIGN_IN_SUCCESS;
                } else {
                    // Người dùng đã đăng nhập từ trước
                    return ACCOUNT_BUSY;
                }
            } else {
                return WRONG_PASSWORD;
            }
        }
        user = user->next;
    }

    // Nếu không tìm thấy username
    return USERNAME_NOT_EXISTED;
}
