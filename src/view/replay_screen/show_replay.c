#include <stdio.h>
#include <windows.h>
#include "top_screen.h"
#include "games.h"

int replayId = 0;

// Constants for table layout
int rowHeight = 2; // Height of each row (in console lines)
int tableStartY = 8; // Starting Y coordinate of the table

void drawReplayInfoUI() {
    system("cls");
    
    gotoxy(27, 3);
    printf("Replay Information");
    
    gotoxy(0, 5);
    printf("----------------------------------------------------");

    gotoxy(1, 6);
    printf("|   id   |   player1   |   player2   |   result   |");
    
    gotoxy(0, 7);
    printf("----------------------------------------------------");

    for (int i = 0; i < MAX_REPLAYS; i++) {
        gotoxy(1, tableStartY + i * rowHeight);
        printf("|        |             |             |            |");
    }
    
    gotoxy(0, tableStartY + MAX_REPLAYS * rowHeight);
    printf("----------------------------------------------------");
    
    gotoxy(26, tableStartY + MAX_REPLAYS * rowHeight + 2);
    printf("[ Watch ]");
    
    gotoxy(40, tableStartY + MAX_REPLAYS * rowHeight + 2);
    printf("[ Back ]");
    currentScreen = VIEW_REPLAY_LIST;
}

// void fetchReplayInfoData() {
//     // Simulate fetching replay data (example data)
//     for (int i = 0; i < MAX_REPLAYS; i++) {
//         replayDataArray[i].id = i + 1;
//         sprintf(replayDataArray[i].player1, "Player %d", i + 1);
//         sprintf(replayDataArray[i].player2, "Player %d", MAX_REPLAYS - i);
//         sprintf(replayDataArray[i].result, i % 2 == 0 ? "Win" : "Loss");
//         sprintf(replayDataArray[i].move, "Move %d", i * 10);
//     }
// }

void displayReplayInfoData() {
    for (int i = 0; i < MAX_REPLAYS; i++) {
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
    if (MousePos.Y == (tableStartY + MAX_REPLAYS * rowHeight + 2) && MousePos.X >= 16 && MousePos.X <= 22){
        // Gọi hàm hiển thị màn hình Watch Replay
        DrawReplayBoard();
        // Trả về cho control giá trị của Id (replayId)
        
    }
    else if (MousePos.Y == (tableStartY + MAX_REPLAYS * rowHeight + 2) && MousePos.X >= 30 && MousePos.X <= 36){
        // Gọi hàm hiển thị màn hình Top Logined ở đây
        dashboard();
    }
}