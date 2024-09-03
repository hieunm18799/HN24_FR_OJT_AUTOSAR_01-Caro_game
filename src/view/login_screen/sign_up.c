#include "top_screen.h"
#include "protocol.h"

// Macro definitions for UI elements
#define SIGNIN_X 60
#define SIGNIN_Y 2
#define USERNAME_X 32
#define USERNAME_Y 10
#define PASSWORD_Y 12
#define REENTER_PASSWORD_Y 14
#define BUTTON_X 35
#define SIGNUP_Y 17
#define BACK_Y 19
#define INPUT_X 30
#define INPUT_WIDTH 20

void drawSignUpUI() {
    system("cls");

    // Draw top border
    gotoxy(8, 1);
    printf("+--------------------------------------------------------------+");

    // Draw side borders
    for (int i = 2; i <= 20; i++) {
        gotoxy(8, i);
        printf("|");
        gotoxy(71, i);
        printf("|");
    }

    // Draw bottom border
    gotoxy(8, 21);
    printf("+--------------------------------------------------------------+");

    gotoxy(SIGNIN_X, SIGNIN_Y);
    printf("[ Sign in ]");

    gotoxy(38, 8);
    printf("Caro Game");

    gotoxy(20, USERNAME_Y);
    printf("Username");

    gotoxy(INPUT_X, USERNAME_Y);
    printf("[                    ]");

    gotoxy(20, PASSWORD_Y);
    printf("Password");

    gotoxy(INPUT_X, PASSWORD_Y);
    printf("[                    ]");

    gotoxy(11, REENTER_PASSWORD_Y);
    printf("Re-enter password");

    gotoxy(INPUT_X, REENTER_PASSWORD_Y);
    printf("[                    ]");

    gotoxy(BUTTON_X, SIGNUP_Y);
    printf("[ Sign up ]");

    gotoxy(BUTTON_X + 3, BACK_Y);
    printf("Back");

    currentScreen = VIEW_SIGN_UP; // Set the current screen to the signup screen
}

void handleClickOnSignupScreen() {
    if (MousePos.Y >= USERNAME_Y && MousePos.Y <= REENTER_PASSWORD_Y && MousePos.X >= (INPUT_X + 2) && MousePos.X <= (INPUT_X + INPUT_WIDTH - 2)) {
        enterSignupCredentials(signup_username, signup_password, signup_reenterPassword);
    }
    // When clicking Sign In on the signup screen
    else if (MousePos.Y == SIGNIN_Y && MousePos.X >= SIGNIN_X && MousePos.X <= (SIGNIN_X + 10)) {
        drawSignInUI(); // Open the sign-in screen from the signup screen
    }
    // When clicking Back on the signup screen
    else if (MousePos.Y == BACK_Y && MousePos.X >= BUTTON_X && MousePos.X <= (BUTTON_X + 9)) {
        drawInitialUI(); // Back button opens the initial screen
    }
    // When clicking Sign Up on the signup screen
    else if (MousePos.Y == SIGNUP_Y && MousePos.X >= BUTTON_X && MousePos.X <= (BUTTON_X + 6)) {
        signup(sockfd, signup_username, signup_password, signup_reenterPassword);
    }
}