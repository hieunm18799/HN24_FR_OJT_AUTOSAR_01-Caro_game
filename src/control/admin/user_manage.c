#include "users.h"
#include <stdio.h>
#include <string.h>

// Hàm liệt kê tất cả người dùng
void listUsers() {
    User* current = getUsers();
    printf("Username\tRole\tStatus\tWins\tLosses\tDraws\n");
    while (current != NULL) {
        printf("%s\t\t%s\t%s\t%d\t%d\t%d\n", 
            current->username, current->role, current->status, 
            current->wins, current->losses, current->draws);
        current = current->next;
    }
}

// Hàm xóa người dùng (chỉ admin mới có quyền sử dụng)
void deleteUserByAdmin(const char* username) {
    User* current = getUsers();
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            if (strcmp(current->role, "admin") == 0) {
                printf("Cannot delete an admin user!\n");
                return;
            }
            break;
        }
        current = current->next;
    }
    
    // Gọi hàm deleteUser trong users.c để thực hiện xóa
    deleteUser(username);
    printf("User %s has been deleted.\n", username);
}

// Hàm cập nhật trạng thái người dùng (chỉ admin mới có quyền sử dụng)
void updateUserStatus(const char* username, const char* status) {
    User* current = getUsers();
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            // Không cho phép admin thay đổi trạng thái của chính mình
            if (strcmp(current->role, "admin") == 0 && strcmp(status, "NOT_SIGN_IN") == 0) {
                printf("Cannot change status of admin!\n");
                return;
            }
            setUserStatus(username, status);
            printf("User %s status has been updated to %s.\n", username, status);
            return;
        }
        current = current->next;
    }
    printf("User %s not found.\n", username);
}
