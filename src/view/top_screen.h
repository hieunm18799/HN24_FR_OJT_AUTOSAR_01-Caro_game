#ifndef TOP_SCREEN_H
#define TOP_SCREEN_H

extern COORD MousePos; // Biến lưu trữ vị trí chuột
extern int Click_flag; // Cờ để xác định xem đã click chuột hay chưa
extern int currentScreen; 

void gotoxy(int x, int y);

void drawInitialUI();
void drawSignInUI();
void drawSignUpUI();

void enterSigninCredentials(char *username, char *password);
void enterSignupCredentials(char *username, char *password, char *reenterPassword);

void handleClickOnInitialScreen();
void handleClickOnSigninScreen();
void handleClickOnSignupScreen();

#endif