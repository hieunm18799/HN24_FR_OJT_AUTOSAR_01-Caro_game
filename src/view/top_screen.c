#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <top_screen.h>
#include <sign_in.h>
#include <sign_up.h>

void drawTopNotLoginScreen() {
    system("cls");  // Clear the console
    printf("************************************************************\n");
    printf("*                                                          *\n");
    printf("*                                                          *\n");
    printf("*                        Caro Game                         *\n");
    printf("*                                                          *\n");
    printf("*                                                          *\n");
    printf("*                                                          *\n");
    printf("*                      ************                        *\n");
    printf("*                      *  Sign in  *                       *\n");
    printf("*                      ************                        *\n");
    printf("*                                                          *\n");
    printf("*                      ************                        *\n");
    printf("*                      *  Sign up  *                       *\n");
    printf("*                      ************                        *\n");
    printf("*                                                          *\n");
    printf("*                                                          *\n");
    printf("************************************************************\n");
}

void drawTopLoginedScreen(){

}

// Function to handle user input for username and password during login
void enterCredentials(char *username, char *password) {
    COORD coord = {12, 6};  // Position of the username input field in the console
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    scanf("%s", username);

    coord.Y = 7;  // Position of the password input field in the console
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    // Handle password input to hide the characters
    int i = 0;
    char ch;
    while (1) {
        ch = _getch();  // Get input without echoing to console
        if (ch == 13) { // Enter key
            password[i] = '\0';
            break;
        } else if (ch == 8) { // Backspace key
            if (i > 0) {
                i--;
                printf("\b \b");  // Move back, overwrite with space, move back again
            }
        } else {
            password[i] = ch;
            i++;
            printf("*");  // Print asterisks instead of the actual character
        }
    }
    printf("\n%s",username);
    printf("\n%s",password);
}

// Function to process login screen events
void processLoginScreen() {
    // Add the logic to handle login screen clicks and inputs here
    HANDLE hInput;
    DWORD events;
    INPUT_RECORD inputRecord;
    char signin_username[50];
    char signin_password[50];
    char signup_username[50];
    char signup_password[50];
    char sigup_reenterPassword[50];

    hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

    while (1) {
        ReadConsoleInput(hInput, &inputRecord, 1, &events);
        if (inputRecord.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD mouseEvent = inputRecord.Event.MouseEvent;
            if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                COORD pos = mouseEvent.dwMousePosition;

                // Handle "Sign In" button click
                if (pos.Y == 8 && (pos.X >= 23 && pos.X <= 34)) {
                    printf("You clicked Sign In at (%d, %d)\n", pos.X, pos.Y);
                    drawLoginScreen();
                    enterCredentials(signin_username, signin_password);  // Enter username and password
                    break;
                }
                else if (pos.Y == 12 && (pos.X >= 23 && pos.X <= 34)) {
                    printf("You clicked Sign In at (%d, %d)\n", pos.X, pos.Y);
                    drawSignupScreen();
                    enterSignupCredentials(signup_username, signup_password, sigup_reenterPassword);
                    break;
                }
                else if (pos.Y == 15 && (pos.X >= 28 && pos.X <= 35)){
                    printf("You clicked Back at (%d, %d)\n", pos.X, pos.Y);
                    vdrawTopNotLoginScreen();
                    break;
                }
            }
        }
    }
}

void enterSignupCredentials(char *username, char *password, char *reenterPassword) {
    COORD coord = {12, 6};  // Position of the username input field in the console
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    scanf("%s", username);

    coord.Y = 7;  // Position of the password input field in the console
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    int i = 0;
    char ch;
    while (1) {
        ch = _getch();  // Get input without echoing to console
        if (ch == 13) { // Enter key
            password[i] = '\0';
            break;
        } else if (ch == 8) { // Backspace key
            if (i > 0) {
                i--;
                printf("\b \b");  // Move back, overwrite with space, move back again
            }
        } else {
            password[i] = ch;
            i++;
            printf("*");  // Print asterisks instead of the actual character
        }
    }

    coord.Y = 8;  // Position of the re-enter password input field in the console
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    i = 0;
    while (1) {
        ch = _getch();  // Get input without echoing to console
        if (ch == 13) { // Enter key
            reenterPassword[i] = '\0';
            break;
        } else if (ch == 8) { // Backspace key
            if (i > 0) {
                i--;
                printf("\b \b");  // Move back, overwrite with space, move back again
            }
        } else {
            reenterPassword[i] = ch;
            i++;
            printf("*");  // Print asterisks instead of the actual character
        }
    }
}

