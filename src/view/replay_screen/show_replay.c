#include <stdio.h>
#include <windows.h>
#include "top_screen.h"

#define MAX_REPLAYS 10
int replayId = 0;

// Define structure to hold replay data
typedef struct {
    int id;
    char player1[50];
    char player2[50];
    char result[10];
    char move[50];
} ReplayData;

// Example array and number of replays
ReplayData replayDataArray[MAX_REPLAYS]; // Example array of replays
int numReplays = MAX_REPLAYS; // Number of replays

// Constants for table layout
int rowHeight = 2; // Height of each row (in console lines)
int tableStartY = 8; // Starting Y coordinate of the table

void drawReplayInfoUI() {
    system("cls");
    
    gotoxy(27, 3);
    printf("Replay Information");
    
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
    printf("[ Watch ]");
    
    gotoxy(40, tableStartY + numReplays * rowHeight + 2);
    printf("[ Back ]");
    currentScreen = VIEW_REPLAY_LIST;
}

void fetchReplayInfoData() {
    // Simulate fetching replay data (example data)
    for (int i = 0; i < MAX_REPLAYS; i++) {
        replayDataArray[i].id = i + 1;
        sprintf(replayDataArray[i].player1, "Player %d", i + 1);
        sprintf(replayDataArray[i].player2, "Player %d", MAX_REPLAYS - i);
        sprintf(replayDataArray[i].result, i % 2 == 0 ? "Win" : "Loss");
        sprintf(replayDataArray[i].move, "Move %d", i * 10);
    }
}

void displayReplayInfoData() {
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

void handleOnScreenReplayInfo(){
    if (MousePos.Y == (tableStartY + numReplays * rowHeight + 2) && MousePos.X >= 26 && MousePos.X <= 34){
        // Gọi hàm hiển thị màn hình Watch Replay

        // Trả về cho control giá trị của Id (replayId)
        
    }
    else if (MousePos.Y == (tableStartY + numReplays * rowHeight + 2) && MousePos.X >= 40 && MousePos.X <= 46){
        // Gọi hàm hiển thị màn hình Top Logined ở đây
    }
}