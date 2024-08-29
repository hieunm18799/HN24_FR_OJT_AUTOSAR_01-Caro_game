#include "users.h"
#include "protocol.h"

void sign_in(SOCKET clientSocket) {
    Request req;
    Response res;
    
    // Nhận yêu cầu từ client
    if (recvReq(clientSocket, &req, sizeof(Request), 0) <= 0) {
        printf("Failed to receive sign in request.\n");
        return;
    }

    char username[MAX_LENGTH], password[MAX_LENGTH];
    sscanf(req.message, "%s %s", username, password);

    User* user = userList;
    while (user) {
        if (strcmp(user->username, username) == 0) {
            // Kiểm tra password
            if (strcmp(user->password, password) == 0) {
                if (strcmp(user->status, "NOT_SIGN_IN") == 0) {
                    // Cập nhật trạng thái người dùng
                    setUserStatus(username, "SIGNED_IN");
                    user->clientfd = clientSocket;

                    // Phản hồi đăng nhập thành công
                    res.code = SIGN_IN_SUCCESS;
                    strcpy(res.data, username);
                    setMessageResponse(&res);
                    sendRes(clientSocket, &res, sizeof(Response), 0);
                } else {
                    // Người dùng đã đăng nhập từ trước
                    res.code = ACCOUNT_BUSY;
                    setMessageResponse(&res);
                    sendRes(clientSocket, &res, sizeof(Response), 0);
                }
                return;
            } else {
                // Sai password
                res.code = WRONG_PASSWORD;
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
