#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "top_screen.h"
#include "games.h"

int replayId = 0;

static rowHeight = 2; // Height of each row (in console lines)
static int tableStartY = 8; // Starting Y coordinate of the table

// Function to draw the replay management UI
void drawReplayManagementUI() {
    system("cls");
    
    gotoxy(27, 3);
    printf("Admin Dashboard");
    
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
    
    gotoxy(16, tableStartY + MAX_REPLAYS * rowHeight + 2);
    printf("[ Delete ]");
    
    gotoxy(30, tableStartY + MAX_REPLAYS * rowHeight + 2);
    printf("[ Back ]");
    currentScreen = VIEW_ADMIN_REPLAY_MANAGE;
}

// // Function to fetch replay data from the server
// void fetchReplayData() {
//     // Simulate fetching replay data (example data)
//     for (int i = 0; i < MAX_REPLAYS; i++) {
//         replayDataArray[i].id = i + 1;
//         sprintf(replayDataArray[i].player1, "Player %d", i + 1);
//         sprintf(replayDataArray[i].player2, "Player %d", MAX_REPLAYS - i);
//         sprintf(replayDataArray[i].result, i % 2 == 0 ? "Win" : "Loss");
//     }
// }

// Function to display replay data in the table
void displayReplayData() {
    for (int i = 0; i < MAX_REPLAYS; i++) {
        gotoxy(3, tableStartY + i * rowHeight);
        printf("%02d", replayDataArray[i].id);
        
        gotoxy(13, tableStartY + i * rowHeight);
        printf("%-9s", replayDataArray[i].player1);
        
        gotoxy(28, tableStartY + i * rowHeight);
        printf("%-9s", replayDataArray[i].player2);
        
        gotoxy(43, tableStartY + i * rowHeight);
        printf("%-7s", replayDataArray[i].result);
    }
}


// Function to handle row clicks and determine the selected row and ID
void handleRowClick() {

    // Determine which row was clicked
    if (MousePos.Y >= tableStartY && MousePos.Y < tableStartY + (MAX_REPLAYS * rowHeight)) {
        int clickedRow = (MousePos.Y - tableStartY) / rowHeight;

        if (clickedRow < MAX_REPLAYS) {
            replayId = replayDataArray[clickedRow].id;
        } 
    }
    
}

void handleOnScreenReplayManagement(){

    if (MousePos.Y == (tableStartY + MAX_REPLAYS * rowHeight + 2) && MousePos.X >= 16 && MousePos.X <= 22){
        // Example: Call delete function (You can change to other operations)
        deleteReplay(replayId);
    }
    else if (MousePos.Y == (tableStartY + MAX_REPLAYS * rowHeight + 2) && MousePos.X >= 30 && MousePos.X <= 36){
        // Giải phóng bộ nhớ động khi không còn sử dụng
        free(replayDataArray);
        // Gọi hàm frameScreenAdmin ở đây
        // frameScreenAdmin();
    }
}

void deleteReplay(int replayId) {
    int foundIndex = -1;

    // Step 1: Find the replay by its ID
    for (int i = 0; i < MAX_REPLAYS; i++) {
        if (replayDataArray[i].id == replayId) {
            foundIndex = i;
            break;
        }
    }

    // Step 2: If the replay is found, delete it
    if (foundIndex != -1) {
        // Step 3: Shift remaining replays to fill the gap
        for (int i = foundIndex; i < MAX_REPLAYS - 1; i++) {
            replayDataArray[i] = replayDataArray[i + 1];
        }

        // Step 4: Decrease the number of replays
        MAX_REPLAYS--;

        // Step 5: Clear the last replay (optional, for cleanliness)
        replayDataArray[MAX_REPLAYS].id = 0;
        replayDataArray[MAX_REPLAYS].player1[0] = '\0';
        replayDataArray[MAX_REPLAYS].player2[0] = '\0';
        replayDataArray[MAX_REPLAYS].result[0] = '\0';

        // Step 6: Update the UI
        drawReplayManagementUI();
        displayReplayData();
        
        // Step 7: Send updated data to server (if needed)
        // Gọi hàm gửi dữ liệu lên server ở đây

    }
}