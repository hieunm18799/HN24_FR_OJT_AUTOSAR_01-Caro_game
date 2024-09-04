#include "users.h"
#include "protocol.h"

RES_OPCODE sign_up(char* username, char* password, char* confirmPassword) {
    // Kiểm tra xem username đã tồn tại chưa
    if (username == NULL || password == NULL || confirmPassword == NULL || strcmp(password, confirmPassword) != 0)
        return SIGN_UP_INPUT_WRONG;
    User* user = getUsers();
    while (user) {
        if (strcmp(user->username, username) == 0) {
            return USERNAME_EXISTED;
        }
        user = user->next;
    }

    // Tạo người dùng mới
    newUser(username, password, "default", 0, 0, 0);
    writeUsersIni();

    // Gửi phản hồi thành công
    return SIGN_UP_SUCCESS;
}
