#include "top_screen.h"

#define CARO_GAME_STRING_POSITION_X 10
#define CARO_GAME_STRING_POSITION_Y 0
#define FINDING_GAME_POSITION_X 10
#define FINDING_GAME_POSITION_Y 5
#define BACK_POSITION_X 10
#define BACK_POSITION_Y 10
#define BUTTON_WIDTH 10

COORD CursorPosition;

// void handleMouseClick() {
//     HANDLE hInput;
//     DWORD events;
//     INPUT_RECORD inputRecord;

//     hInput = GetStdHandle(STD_INPUT_HANDLE);
//     SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

//     // Read mouse input
//     if (ReadConsoleInput(hInput, &inputRecord, 1, &events)) {
//         if (inputRecord.EventType == MOUSE_EVENT) {
//             MOUSE_EVENT_RECORD mouseEvent = inputRecord.Event.MouseEvent;
//             if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
//                 MousePos = mouseEvent.dwMousePosition; // Save the mouse position
//                 Click_flag = 1;  // Set the click flag to indicate a click occurred
//                 Sleep(100); // Debounce delay to avoid multiple captures of the same click
//             }
//         }
//     }
// }

void drawFindPlayer() {
    currentScreen = VIEW_FIND_GAME;
    system("cls");
    gotoxy(CARO_GAME_STRING_POSITION_X,CARO_GAME_STRING_POSITION_Y);
    printf("CARO GAME");

    gotoxy(FINDING_GAME_POSITION_X,FINDING_GAME_POSITION_Y);
    printf("FINDING GAME");

    gotoxy(BACK_POSITION_X,BACK_POSITION_Y);

    printf("[ BACK ]");
}


void handleClickOnFindPlayerScreen() {
    if (Click_flag == 1) {
        Click_flag = 0;
        // Check if the "BACK" button was clicked
        if (MousePos.Y == BACK_POSITION_Y && MousePos.X >= BACK_POSITION_X && MousePos.X <= (BACK_POSITION_X + BUTTON_WIDTH)) {
            system("cls");
            
        }
    }
}


// int main () {
//     drawFindPlayer(); 
//     getchar();
//     getchar();
    
// }