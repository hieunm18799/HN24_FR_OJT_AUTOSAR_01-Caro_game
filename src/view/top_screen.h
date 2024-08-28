#ifndef TOP_SCREEN_H
#define TOP_SCREEN_H

COORD MousePos; // Biến lưu trữ vị trí chuột
int Click_flag = 0; // Cờ để xác định xem đã click chuột hay chưa
int currentScreen = 0; 
char signin_username[50];
char signin_password[50];
char signup_username[50];
char signup_password[50];
char signup_reenterPassword[50];

void drawInitialUI();
void drawSignInUI();
void drawSignUpUI();

void enterSigninCredentials(char *username, char *password);
void enterSignupCredentials(char *username, char *password, char *reenterPassword);

void handleClickOnInitialScreen();
void handleClickOnSigninScreen();
void handleClickOnSignupScreen();

#endif