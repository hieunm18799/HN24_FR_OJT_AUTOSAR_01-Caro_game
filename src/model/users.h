#ifndef USER_H
#define USER_H

#include <stdbool.h>
#include <winsock2.h>

#define MAX_LINE_LENGTH 256

// Cấu trúc dữ liệu của người dùng
typedef struct User {
    char username[50];
    char password[50];
    char role[50];
    char status[50];
    SOCKET clientfd; // Thay đổi kiểu dữ liệu từ int sang SOCKET
    int wins;
    int losses;
    int draws;
    struct User* next;
} User;

static char userFileName[] = "Users.ini";

// Hàm quản lý người dùng
void initializeUser();                        // Khởi tạo danh sách người dùng từ Users.ini
User *createUser(const char* username, const char* password, const char* role, unsigned int wins, unsigned int losses, unsigned int draws);
void newUser(const char* username, const char* password, const char* role, unsigned int wins, unsigned int losses, unsigned int draws);  // Tạo người dùng mới
void changeUser(User *user, const char* username, const char* password, const char* role, unsigned int wins, unsigned int losses, unsigned int draws);  // Tạo người dùng mới
bool setUserStatus(const char* username, const char* status);  // Cập nhật trạng thái người dùng
void increasedWins(User *user);
void increasedLosses(User *user);
User *findUserByName(const char* username);
int deleteUser(const char* username);       // Xóa người dùng khỏi danh sách và cập nhật tệp INI
User* getUsers();                            // Lấy danh sách người dùng
char* getUserRole(const char* username);      // Lấy role người dùng
void freeUsers();
void logoutUsers();

void readUsersIni();
void writeUsersIni();

#endif // USER_H
