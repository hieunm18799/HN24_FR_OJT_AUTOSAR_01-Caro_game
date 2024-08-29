#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "top_screen.h"

typedef struct {
    int id;
    char player1[50];
    char player2[50];
    char result[10];
    char move[50];
} ReplayData;

#define MAX_REPLAYS 15
static numReplays = MAX_REPLAYS; // Number of replays
static int replayId = 0;
static rowHeight = 2; // Height of each row (in console lines)
static int tableStartY = 8; // Starting Y coordinate of the table
// Example array and number of replays
ReplayData replayDataArray[MAX_REPLAYS]; // Example array of replays

// Function to draw the replay management UI
void drawReplayManagementUI() {
    system("cls");
    
    gotoxy(27, 3);
    printf("Admin Dashboard");
    
    gotoxy(0, 5);
    printf("---------------------------------------------------------------------");

    gotoxy(1, 6);
    printf("|   id   |   player1   |   player2   |   result   |   move            |");
    
    gotoxy(0, 7);
    printf("---------------------------------------------------------------------");

    for (int i = 0; i < MAX_REPLAYS; i++) {
        gotoxy(1, tableStartY + i * rowHeight);
        printf("|        |             |             |            |                  |");
    }
    
    gotoxy(0, tableStartY + numReplays * rowHeight);
    printf("---------------------------------------------------------------------");
    
    gotoxy(26, tableStartY + numReplays * rowHeight + 2);
    printf("[ Delete ]");
    
    gotoxy(40, tableStartY + numReplays * rowHeight + 2);
    printf("[ Back ]");
    currentScreen = VIEW_ADMIN_REPLAY_MANAGE;
}

// Function to fetch replay data from the server
void fetchReplayData() {
    // Simulate fetching replay data (example data)
    for (int i = 0; i < MAX_REPLAYS; i++) {
        replayDataArray[i].id = i + 1;
        sprintf(replayDataArray[i].player1, "Player %d", i + 1);
        sprintf(replayDataArray[i].player2, "Player %d", MAX_REPLAYS - i);
        sprintf(replayDataArray[i].result, i % 2 == 0 ? "Win" : "Loss");
        sprintf(replayDataArray[i].move, "Move %d", i * 10);
    }
}

// Function to display replay data in the table
void displayReplayData() {
    for (int i = 0; i < numReplays; i++) {
        gotoxy(3, tableStartY + i * rowHeight);
        printf("%02d", replayDataArray[i].id);
        
        gotoxy(13, tableStartY + i * rowHeight);
        printf("%-9s", replayDataArray[i].player1);
        
        gotoxy(28, tableStartY + i * rowHeight);
        printf("%-9s", replayDataArray[i].player2);
        
        gotoxy(43, tableStartY + i * rowHeight);
        printf("%-7s", replayDataArray[i].result);
        
        gotoxy(55, tableStartY + i * rowHeight);
        printf("%-14s", replayDataArray[i].move);
    }
}

// Function to handle mouse clicks and store the mouse position
void handleMouseClick() {
    HANDLE hInput;
    DWORD events;
    INPUT_RECORD inputRecord;

    hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

    // Read mouse input
    if (ReadConsoleInput(hInput, &inputRecord, 1, &events)) {
        if (inputRecord.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD mouseEvent = inputRecord.Event.MouseEvent;
            if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                MousePos = mouseEvent.dwMousePosition; // Save the mouse position
                Click_flag = 1;  // Set the click flag to indicate a click occurred
                Sleep(100); // Debounce delay to avoid multiple captures of the same click
            }
        }
    }
}

// Function to handle row clicks and determine the selected row and ID
void handleRowClick() {

    // Determine which row was clicked
    if (MousePos.Y >= tableStartY && MousePos.Y < tableStartY + (numReplays * rowHeight)) {
        int clickedRow = (MousePos.Y - tableStartY) / rowHeight;

        if (clickedRow < numReplays) {
            replayId = replayDataArray[clickedRow].id;
        } 
    }
    
}

void handleOnScreenReplayManagement(){

    if (MousePos.Y == (tableStartY + numReplays * rowHeight + 2) && MousePos.X >= 26 && MousePos.X <= 34){
        // Example: Call delete function (You can change to other operations)
        deleteReplay(replayId);
    }
    else if (MousePos.Y == (tableStartY + numReplays * rowHeight + 2) && MousePos.X >= 40 && MousePos.X <= 46){
        // Gọi hàm frameScreenAdmin ở đây
        // Button Back
        frameScreenAdmin();
    }
}

void deleteReplay(int replayId) {
    int foundIndex = -1;

    // Step 1: Find the replay by its ID
    for (int i = 0; i < numReplays; i++) {
        if (replayDataArray[i].id == replayId) {
            foundIndex = i;
            break;
        }
    }

    // Step 2: If the replay is found, delete it
    if (foundIndex != -1) {
        // Step 3: Shift remaining replays to fill the gap
        for (int i = foundIndex; i < numReplays - 1; i++) {
            replayDataArray[i] = replayDataArray[i + 1];
        }

        // Step 4: Decrease the number of replays
        numReplays--;

        // Step 5: Clear the last replay (optional, for cleanliness)
        replayDataArray[numReplays].id = 0;
        replayDataArray[numReplays].player1[0] = '\0';
        replayDataArray[numReplays].player2[0] = '\0';
        replayDataArray[numReplays].result[0] = '\0';
        replayDataArray[numReplays].move[0] = '\0';

        // Step 6: Update the UI
        drawReplayManagementUI();
        displayReplayData();
        
        // Step 7: Send updated data to server (if needed)
        sendDataToServer();
    }
}

// Function to send data back to the server after deletion
void sendDataToServer() {
    // Simulate sending updated data to the server
    printf("Data sent back to the server.\n");
}