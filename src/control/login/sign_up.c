#include "users.h"
#include "protocol.h"

void sign_up(SOCKET clientSocket) {
    Request req;
    Response res;
    
    // Nhận yêu cầu từ client
    if (recvReq(clientSocket, &req, sizeof(Request), 0) <= 0) {
        printf("Failed to receive sign up request.\n");
        return;
    }

    char username[MAX_LENGTH], password[MAX_LENGTH];
    sscanf(req.message, "%s %s", username, password);

    // Kiểm tra xem username đã tồn tại chưa
    User* user = userList;
    while (user) {
        if (strcmp(user->username, username) == 0) {
            res.code = USERNAME_EXISTED;
            setMessageResponse(&res);
            sendRes(clientSocket, &res, sizeof(Response), 0);
            return;
        }
        user = user->next;
    }

    // Tạo người dùng mới
    newUser(username, password, "default");
    writeUsersIni("Users.ini");

    // Gửi phản hồi thành công
    res.code = SIGN_UP_SUCCESS;
    setMessageResponse(&res);
    sendRes(clientSocket, &res, sizeof(Response), 0);
}
