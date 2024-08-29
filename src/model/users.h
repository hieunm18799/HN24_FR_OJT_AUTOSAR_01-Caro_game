#ifndef USER_H
#define USER_H

#include <stdbool.h>
#include <winsock2.h>

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

extern User* userList;

// Hàm quản lý người dùng
void initializeUser();                        // Khởi tạo danh sách người dùng từ Users.ini
bool validateUser(const char* username, const char* password);  // Xác thực người dùng
void newUser(const char* username, const char* password, const char* role);  // Tạo người dùng mới
bool setUserStatus(const char* username, const char* status);  // Cập nhật trạng thái người dùng
void writeUsersIni(const char* filename);    // Ghi danh sách người dùng vào tệp INI
void deleteUser(const char* username);       // Xóa người dùng khỏi danh sách và cập nhật tệp INI
User* getUsers();                            // Lấy danh sách người dùng
char* getUserRole(const char* username);      // Lấy role người dùng

// Các hàm sign_in, sign_up, sign_out
void sign_in(SOCKET clientSocket);
void sign_up(SOCKET clientSocket);
void sign_out(SOCKET clientSocket);

// Các hàm quản lý người dùng
void listUsers();
void deleteUserByAdmin(const char* username);
void updateUserStatus(const char* username, const char* status);

// Hàm đọc file Users.ini
void readUsersIni(const char* filename);

#endif // USER_H
