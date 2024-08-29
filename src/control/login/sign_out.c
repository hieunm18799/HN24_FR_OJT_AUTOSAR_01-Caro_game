#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include "users.h"
#include "protocol.h"

#define USERS_FILE "Users.ini"

RES_OPCODE sign_out(char *username) {
    // Cập nhật trạng thái người dùng
    if (setUserStatus(username, "NOT_SIGN_IN")) {
        writeUsersIni(USERS_FILE);
        return SIGN_OUT_SUCCESS;
    }
    return SIGN_OUT_FAIL;
}
