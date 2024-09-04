#include "top_screen.h"
#include "protocol.h"

// Macro definitions for coordinates and labels
#define SIGNUP_X 60
#define SIGNUP_Y 2
#define TITLE_X 38
#define TITLE_Y 8
#define LABEL_X 20
#define INPUT_X 30
#define USERNAME_Y 10
#define PASSWORD_Y 12
#define SIGNIN_X 35
#define SIGNIN_Y 15
#define BACK_X 38
#define BACK_Y 17

#define LABEL_WIDTH 20
#define INPUT_WIDTH 20

// Macro definitions for UI labels
#define TITLE "Caro Game"
#define SIGNUP_LABEL "[ Sign up ]"
#define USERNAME_LABEL "Username"
#define PASSWORD_LABEL "Password"
#define SIGNIN_LABEL "[ Sign in ]"
#define BACK_LABEL "Back"

void drawSignInUI() {
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

    gotoxy(SIGNUP_X, SIGNUP_Y);
    printf(SIGNUP_LABEL);

    gotoxy(TITLE_X, TITLE_Y);
    printf(TITLE);

    gotoxy(LABEL_X, USERNAME_Y);
    printf(USERNAME_LABEL);

    gotoxy(INPUT_X, USERNAME_Y);
    printf("[ %*s ]", INPUT_WIDTH - 2, "");

    gotoxy(LABEL_X, PASSWORD_Y);
    printf(PASSWORD_LABEL);

    gotoxy(INPUT_X, PASSWORD_Y);
    printf("[ %*s ]", INPUT_WIDTH - 2, "");

    gotoxy(SIGNIN_X, SIGNIN_Y);
    printf(SIGNIN_LABEL);

    gotoxy(BACK_X, BACK_Y);
    printf(BACK_LABEL);

    currentScreen = VIEW_SIGN_IN; // Set the current screen to the sign-in screen
}

void handleClickOnSigninScreen() {
    if (MousePos.Y >= USERNAME_Y && MousePos.Y <= PASSWORD_Y && MousePos.X >= (INPUT_X + 2) && MousePos.X <= (INPUT_X + INPUT_WIDTH - 2)) {
        enterSigninCredentials(signin_username, signin_password);
    } 
    else if (MousePos.Y == SIGNUP_Y && MousePos.X >= SIGNUP_X && MousePos.X <= (SIGNUP_X + 10)) {
        drawSignUpUI(); // Open the sign-up screen from the sign-in screen
    } 
    else if (MousePos.Y == BACK_Y && MousePos.X >= BACK_X && MousePos.X <= (BACK_X + 6)) {
        drawInitialUI(); // Button "Back" opens the initial screen
    } 
    else if (MousePos.Y == SIGNIN_Y && MousePos.X >= SIGNIN_X && MousePos.X <= (SIGNIN_X + 10)) {
        signin(sockfd, signin_username, signin_password); // Handle sign-in
    }
}

void clearMessageLine() {
    gotoxy(10, 20);
    for (int i = 0; i < 50; i++) {
        printf(" "); // Ghi đè bằng khoảng trắng
    }
}

void showErrorNotification(const char* errorMessage) {
    clearMessageLine();
    gotoxy(10, 20);
    printf("Error: %s", errorMessage);
}