#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "top_screen.h"

COORD MousePos; // Biến lưu trữ vị trí chuột
int Click_flag = 0; // Cờ để xác định xem đã click chuột hay chưa
int currentScreen = VIEW_TOP_NOT_SIGN_IN;
char signed_in_role[50];
char signed_in_username[50];
char signin_username[50];
char signin_password[50];
char signup_username[50];
char signup_password[50];
char signup_reenterPassword[50];

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void drawInitialUI() {
    system("cls");
    gotoxy(38, 8);
    printf("Caro Game");
    
    gotoxy(35, 10);
    printf("[ Sign in ]");
    
    gotoxy(35, 12);
    printf("[ Sign up ]");

    currentScreen = VIEW_TOP_NOT_SIGN_IN; // Đặt màn hình hiện tại là màn hình ban đầu
}

// Hàm xử lý sự kiện click chuột
void handleMouseClick() {
    HANDLE hInput;
    DWORD events;
    INPUT_RECORD inputRecord;

    hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

    // Đọc sự kiện chuột
    if (ReadConsoleInput(hInput, &inputRecord, 1, &events)) {
        if (inputRecord.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD mouseEvent = inputRecord.Event.MouseEvent;
            if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                MousePos = mouseEvent.dwMousePosition; // Lưu vị trí chuột
                Click_flag = 1;  // Đặt cờ click để chỉ ra rằng đã có sự kiện click
                Sleep(100); // Tránh đọc nhiều lần cùng một sự kiện click
            }
        }
    }
}

void handleClickOnInitialScreen() {
    if (MousePos.Y == 10 && MousePos.X >= 35 && MousePos.X <= 45) {
        drawSignInUI(); // Mở giao diện đăng nhập
    } else if (MousePos.Y == 12 && MousePos.X >= 35 && MousePos.X <= 45) {
        drawSignUpUI(); // Mở giao diện đăng ký
    }
}

void enterSigninCredentials(char *username, char *password) {
    char ch;
    int i = 0;

    // Input username
    gotoxy(32, 10);  // Position of the username input field
    i = 0;
    while ((ch = _getch()) != '\r') { // Enter key
        if (ch == '\b') { // Backspace key
            if (i > 0) {
                i--;
                printf("\b \b"); // Move back, overwrite with space, move back again
            }
        } else if (ch != 0 && ch != '\xE0') { // Ignore special keys
            username[i] = ch;
            i++;
            printf("%c", ch); // Echo the character
        }
    }
    username[i] = '\0'; // Null-terminate the string
    printf("\n");

    // Input password
    gotoxy(32, 12);  // Position of the password input field
    i = 0;
    while (1) {
        ch = _getch(); // Get input without echoing to console
        if (ch == '\r') { // Enter key
            password[i] = '\0'; // Null-terminate the string
            break;
        } else if (ch == '\b') { // Backspace key
            if (i > 0) {
                i--;
                printf("\b \b"); // Move back, overwrite with space, move back again
            }
        } else {
            password[i] = ch;
            i++;
            printf("*"); // Print asterisks instead of the actual character
        }
    }
    printf("\n");
}

void enterSignupCredentials(char *username, char *password, char *reenterPassword) {
    char ch;
    int i = 0;

    // Input username
    gotoxy(32, 10);  // Position of the username input field
    i = 0;
    while ((ch = _getch()) != '\r') { // Enter key
        if (ch == '\b') { // Backspace key
            if (i > 0) {
                i--;
                printf("\b \b"); // Move back, overwrite with space, move back again
            }
        } else if (ch != 0 && ch != '\xE0') { // Ignore special keys
            username[i] = ch;
            i++;
            printf("%c", ch); // Echo the character
        }
    }
    username[i] = '\0'; // Null-terminate the string
    printf("\n");

    // Input password
    gotoxy(32, 12);  // Position of the password input field
    i = 0;
    while (1) {
        ch = _getch(); // Get input without echoing to console
        if (ch == '\r') { // Enter key
            password[i] = '\0'; // Null-terminate the string
            break;
        } else if (ch == '\b') { // Backspace key
            if (i > 0) {
                i--;
                printf("\b \b"); // Move back, overwrite with space, move back again
            }
        } else {
            password[i] = ch;
            i++;
            printf("*"); // Print asterisks instead of the actual character
        }
    }
    printf("\n");

    gotoxy(32, 14);  // Position of the password input field
    i = 0;
    while (1) {
        ch = _getch(); // Get input without echoing to console
        if (ch == '\r') { // Enter key
            reenterPassword[i] = '\0'; // Null-terminate the string
            break;
        } else if (ch == '\b') { // Backspace key
            if (i > 0) {
                i--;
                printf("\b \b"); // Move back, overwrite with space, move back again
            }
        } else {
            reenterPassword[i] = ch;
            i++;
            printf("*"); // Print asterisks instead of the actual character
        }
    }
    printf("\n");
}