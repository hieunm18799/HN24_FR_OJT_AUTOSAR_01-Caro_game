#include "users.h"
#include "protocol.h"
#include <stdio.h>
#include <string.h>

RES_OPCODE adminAddUser(char *username, char *password, char* role, unsigned int wins, unsigned int losses, unsigned int draws) {
    if (username == NULL || password == NULL) return ADD_USER_FAIL;
    User* user = findUserByName(username);
    if (user != NULL) return ADD_USER_FAIL;

    // Tạo người dùng mới
    newUser(username, password, role, wins, losses, draws);
    writeUsersIni();
    return ADD_USER_SUCCESS;
}

// Hàm xóa người dùng (chỉ admin mới có quyền sử dụng)
RES_OPCODE adminDeleteUser(const char* username) {
    if (username == NULL) return DELETE_USER_FAIL;
    User* current = getUsers();
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            if (strcmp(current->role, "admin") == 0) {
                printf("Cannot delete an admin user!\n");
                return DELETE_USER_FAIL;
            }
            break;
        }
        current = current->next;
    }
    
    // Gọi hàm deleteUser trong users.c để thực hiện xóa
    if (!deleteUser(username)) return DELETE_USER_FAIL;
    writeUsersIni();
    printf("User %s has been deleted.\n", username);
}

// Hàm cập nhật trạng thái người dùng (chỉ admin mới có quyền sử dụng)
RES_OPCODE adminEditUser(char *username, char *password, char* role, unsigned int wins, unsigned int losses, unsigned int draws) {
    if (username == NULL || password == NULL) return EDIT_USER_FAIL;
    User* current = getUsers();
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            // // Không cho phép admin thay đổi trạng thái của chính mình
            // if (strcmp(current->role, "admin") == 0 && strcmp(status, "NOT_SIGN_IN") == 0) {
            //     printf("Cannot change status of admin!\n");
            //     return;
            // }
            changeUser(current, username, password, role, wins, losses, draws);
            return EDIT_USER_SUCCESS;
        }
        current = current->next;
    }
    return EDIT_USER_FAIL;
}
