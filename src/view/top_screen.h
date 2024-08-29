#ifndef TOP_SCREEN_H
#define TOP_SCREEN_H

enum SCREEN {
    VIEW_TOP_NOT_SIGN_IN,
    VIEW_TOP_SIGNED_IN_ADMIN,
    VIEW_TOP_SIGNED_IN_USER,
    VIEW_SIGN_IN,
    VIEW_SIGN_UP,
    VIEW_FIND_GAME,
    VIEW_PLAY_GAME,
    VIEW_REPLAY_LIST,
    VIEW_WATCH_REPLAY,
    VIEW_ADMIN_MANAGE,
    VIEW_ADMIN_USER_MANAGE,
    VIEW_ADMIN_REPLAY_MANAGE,
};

extern COORD MousePos; // Biến lưu trữ vị trí chuột
extern int Click_flag; // Cờ để xác định xem đã click chuột hay chưa
extern int currentScreen;
extern char signed_in_role[50];
extern char signed_in_username[50];
extern char signin_username[50];
extern char signin_password[50];
extern char signup_username[50];
extern char signup_password[50];
extern char signup_reenterPassword[50];
extern sockfd;

void gotoxy(int x, int y);

void drawInitialUI();
void drawSignInUI();
void drawSignUpUI();

void enterSigninCredentials(char *username, char *password);
void enterSignupCredentials(char *username, char *password, char *reenterPassword);

void handleClickOnInitialScreen();
void handleClickOnSigninScreen();
void handleClickOnSignupScreen();

void handleRowClick();
void fetchReplayData();
void drawReplayManagementUI();
void handleOnScreenReplayManagement();
void deleteReplay(int replayId);
void sendDataToServer();

void handleMouseClick();

void frameScreenAdmin();

void drawReplayInfoUI();
void fetchReplayInfoData();
void displayReplayInfoData();
void handleOnScreenReplayInfo();

#endif