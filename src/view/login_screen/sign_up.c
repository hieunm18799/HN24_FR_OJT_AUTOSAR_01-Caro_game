#include "top_screen.h"
#include "protocol.h"

// Macro definitions for UI elements
#define SIGNIN_X 60
#define SIGNIN_Y 2
#define USERNAME_X 30
#define USERNAME_Y 10
#define PASSWORD_X 30
#define PASSWORD_Y 12
#define REENTER_PASSWORD_X 30
#define REENTER_PASSWORD_Y 14
#define BUTTON_X 35
#define SIGNUP_Y 17
#define BACK_Y 19

void drawSignUpUI() {
    system("cls");
    gotoxy(SIGNIN_X, SIGNIN_Y);
    printf("[ Sign in ]");

    gotoxy(38, 8);
    printf("Caro Game");

    gotoxy(20, USERNAME_Y);
    printf("Username");

    gotoxy(USERNAME_X, USERNAME_Y);
    printf("[                    ]");

    gotoxy(20, PASSWORD_Y);
    printf("Password");

    gotoxy(PASSWORD_X, PASSWORD_Y);
    printf("[                    ]");

    gotoxy(20, REENTER_PASSWORD_Y);
    printf("Re-enter password");

    gotoxy(REENTER_PASSWORD_X, REENTER_PASSWORD_Y);
    printf("[                    ]");

    gotoxy(BUTTON_X, SIGNUP_Y);
    printf("[ Sign up ]");

    gotoxy(BUTTON_X + 3, BACK_Y);
    printf("Back");

    currentScreen = VIEW_SIGN_UP; // Set the current screen to the signup screen
}

void handleClickOnSignupScreen() {
    if (MousePos.Y >= USERNAME_Y && MousePos.Y <= REENTER_PASSWORD_Y && MousePos.X >= (USERNAME_X + 2) && MousePos.X <= (USERNAME_X + 21)) {
        enterSignupCredentials(signup_username, signup_password, signup_reenterPassword);
    }
    // When clicking Sign In on the signup screen
    else if (MousePos.Y == SIGNIN_Y && MousePos.X >= SIGNIN_X && MousePos.X <= (SIGNIN_X + 10)) {
        drawSignInUI(); // Open the sign-in screen from the signup screen
    }
    // When clicking Back on the signup screen
    else if (MousePos.Y == BACK_Y && MousePos.X >= BUTTON_X && MousePos.X <= (BUTTON_X + 5)) {
        drawInitialUI(); // Back button opens the initial screen
    }
    // When clicking Sign Up on the signup screen
    else if (MousePos.Y == SIGNUP_Y && MousePos.X >= BUTTON_X && MousePos.X <= (BUTTON_X + 5)) {
        signup(sockfd, signup_username, signup_password, signup_reenterPassword);
    }
}