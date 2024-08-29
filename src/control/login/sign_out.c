#include "users.h"
#include "protocol.h"

void sign_out(SOCKET clientSocket) {
    Request req;
    Response res;

    // Nhận yêu cầu từ client
    if (recvReq(clientSocket, &req, sizeof(Request), 0) <= 0) {
        printf("Failed to receive sign out request.\n");
        return;
    }

    char username[MAX_LENGTH];
    sscanf(req.message, "%s", username);

    User* user = userList;
    while (user) {
        if (strcmp(user->username, username) == 0) {
            if (user->clientfd == clientSocket) {
                // Cập nhật trạng thái người dùng
                setUserStatus(username, "NOT_SIGN_IN");
                user->clientfd = INVALID_SOCKET;

                // Phản hồi đăng xuất thành công
                res.code = SIGN_OUT_SUCCESS;
                setMessageResponse(&res);
                sendRes(clientSocket, &res, sizeof(Response), 0);
                return;
            } else {
                // Nếu người dùng không đăng nhập từ socket này
                res.code = SIGN_OUT_FAIL;
                setMessageResponse(&res);
                sendRes(clientSocket, &res, sizeof(Response), 0);
                return;
            }
        }
        user = user->next;
    }

    // Nếu không tìm thấy username
    res.code = USERNAME_NOT_EXISTED;
    setMessageResponse(&res);
    sendRes(clientSocket, &res, sizeof(Response), 0);
}
