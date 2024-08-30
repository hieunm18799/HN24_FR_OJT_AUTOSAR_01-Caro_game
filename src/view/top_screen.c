#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "top_screen.h"

// Macro definitions for UI elements
#define USERNAME_X 32
#define USERNAME_Y 10
#define PASSWORD_X 32
#define PASSWORD_Y 12
#define REENTER_PASSWORD_X 32
#define REENTER_PASSWORD_Y 14
#define BUTTON_X 35
#define SIGNIN_Y 10
#define SIGNUP_Y 12

COORD MousePos; // Stores mouse position
int Click_flag = 0; // Flag to detect mouse click
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
    
    gotoxy(BUTTON_X, SIGNIN_Y);
    printf("[ Sign in ]");
    
    gotoxy(BUTTON_X, SIGNUP_Y);
    printf("[ Sign up ]");

    currentScreen = VIEW_TOP_NOT_SIGN_IN; // Set the current screen to the initial screen
}

// Handle mouse click events
void handleMouseClick() {
    HANDLE hInput;
    DWORD events;
    INPUT_RECORD inputRecord;

    hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

    // Read mouse event
    if (ReadConsoleInput(hInput, &inputRecord, 1, &events)) {
        if (inputRecord.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD mouseEvent = inputRecord.Event.MouseEvent;
            if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                MousePos = mouseEvent.dwMousePosition; // Store mouse position
                Click_flag = 1;  // Set click flag to indicate a mouse click event
                Sleep(100); // Avoid multiple reads of the same click event
            }
        }
    }
}

void handleClickOnInitialScreen() {
    if (MousePos.Y == SIGNIN_Y && MousePos.X >= BUTTON_X && MousePos.X <= (BUTTON_X + 10)) {
        drawSignInUI(); // Open the sign-in screen
    } else if (MousePos.Y == SIGNUP_Y && MousePos.X >= BUTTON_X && MousePos.X <= (BUTTON_X + 10)) {
        drawSignUpUI(); // Open the sign-up screen
    }
}

void enterSigninCredentials(char *username, char *password) {
    char ch;
    int i = 0;

    // Input username
    gotoxy(USERNAME_X, USERNAME_Y);  // Position of the username input field
    i = 0;
    while ((ch = _getch()) != '\r') { // Enter key
        if (ch == '\b' && i > 0) { // Backspace key
            i--;
            printf("\b \b"); // Move back, overwrite with space, move back again
        } else if (ch != 0 && ch != '\xE0') { // Ignore special keys
            username[i++] = ch;
            printf("%c", ch); // Echo the character
        }
    }
    username[i] = '\0'; // Null-terminate the string

    // Input password
    gotoxy(PASSWORD_X, PASSWORD_Y);  // Position of the password input field
    i = 0;
    while (1) {
        ch = _getch(); // Get input without echoing to console
        if (ch == '\r') { // Enter key
            password[i] = '\0'; // Null-terminate the string
            break;
        } else if (ch == '\b' && i > 0) { // Backspace key
            i--;
            printf("\b \b"); // Move back, overwrite with space, move back again
        } else {
            password[i++] = ch;
            printf("*"); // Print asterisks instead of the actual character
        }
    }
}

void enterSignupCredentials(char *username, char *password, char *reenterPassword) {
    char ch;
    int i = 0;

    // Input username
    gotoxy(USERNAME_X, USERNAME_Y);  // Position of the username input field
    i = 0;
    while ((ch = _getch()) != '\r') { // Enter key
        if (ch == '\b' && i > 0) { // Backspace key
            i--;
            printf("\b \b"); // Move back, overwrite with space, move back again
        } else if (ch != 0 && ch != '\xE0') { // Ignore special keys
            username[i++] = ch;
            printf("%c", ch); // Echo the character
        }
    }
    username[i] = '\0'; // Null-terminate the string

    // Input password
    gotoxy(PASSWORD_X, PASSWORD_Y);  // Position of the password input field
    i = 0;
    while (1) {
        ch = _getch(); // Get input without echoing to console
        if (ch == '\r') { // Enter key
            password[i] = '\0'; // Null-terminate the string
            break;
        } else if (ch == '\b' && i > 0) { // Backspace key
            i--;
            printf("\b \b"); // Move back, overwrite with space, move back again
        } else {
            password[i++] = ch;
            printf("*"); // Print asterisks instead of the actual character
        }
    }

    // Input re-enter password
    gotoxy(REENTER_PASSWORD_X, REENTER_PASSWORD_Y);  // Position of the re-enter password input field
    i = 0;
    while (1) {
        ch = _getch(); // Get input without echoing to console
        if (ch == '\r') { // Enter key
            reenterPassword[i] = '\0'; // Null-terminate the string
            break;
        } else if (ch == '\b' && i > 0) { // Backspace key
            i--;
            printf("\b \b"); // Move back, overwrite with space, move back again
        } else {
            reenterPassword[i++] = ch;
            printf("*"); // Print asterisks instead of the actual character
        }
    }
}