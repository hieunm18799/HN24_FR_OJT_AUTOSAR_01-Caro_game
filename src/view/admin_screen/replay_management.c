#include "top_screen.h"
#include "games.h"

#define TITLE_X 27
#define TITLE_Y 3
#define ROW_HEIGHT 2 // Height of each row (in console lines)
#define TABLE_START_Y 8 // Starting Y coordinate of the table
#define TABLE_START_X 0
#define BUTTON_DELETE_X 16
#define BUTTON_BACK_X 30
#define BUTTON_WIDTH 6
#define BUTTON_Y_OFFSET 2
#define ID_COLUMN 3
#define PLAYER1_COLUMN 13
#define PLAYER2_COLUMN 28
#define RESULT_COLUMN 43

int MAX_REPLAYS = 10;
// Function to draw the replay management UI
void drawReplayManagementUI() {
    system("cls");

    // Draw title
    gotoxy(TITLE_X, TITLE_Y);
    printf("Replay Management");

    // Draw table header
    gotoxy(TABLE_START_X, TABLE_START_Y);
    printf("----------------------------------------------------");

    gotoxy(1, TABLE_START_Y + 1);
    printf("|   id   |   player1   |   player2   |   result   |");

    gotoxy(TABLE_START_X, TABLE_START_Y + 2);
    printf("----------------------------------------------------");

    // Draw table rows
    for (int i = 0; i < MAX_REPLAYS; i++) {
        gotoxy(1, TABLE_START_Y + 3 + i * ROW_HEIGHT);
        printf("|        |             |             |            |");
    }

    // Draw table footer
    gotoxy(TABLE_START_X, TABLE_START_Y + 3 + MAX_REPLAYS * ROW_HEIGHT);
    printf("----------------------------------------------------");

    // Draw buttons
    gotoxy(BUTTON_DELETE_X, TABLE_START_Y + 3 + MAX_REPLAYS * ROW_HEIGHT + BUTTON_Y_OFFSET);
    printf("[ Delete ]");

    gotoxy(BUTTON_BACK_X, TABLE_START_Y + 3 + MAX_REPLAYS * ROW_HEIGHT + BUTTON_Y_OFFSET);
    printf("[ Back ]");

    currentScreen = VIEW_ADMIN_REPLAY_MANAGE;
}

// Function to display replay data in the table
void displayReplayData() {
    for (int i = 0; i < MAX_REPLAYS; i++) {
        gotoxy(ID_COLUMN, TABLE_START_Y + 3 + i * ROW_HEIGHT);
        printf("%02d", replayDataArray[i].id);

        gotoxy(PLAYER1_COLUMN, TABLE_START_Y + 3 + i * ROW_HEIGHT);
        printf("%-9s", replayDataArray[i].player1);

        gotoxy(PLAYER2_COLUMN, TABLE_START_Y + 3 + i * ROW_HEIGHT);
        printf("%-9s", replayDataArray[i].player2);

        gotoxy(PLAYER2_COLUMN, TABLE_START_Y + 3 + i * ROW_HEIGHT);
        printf("%-7s", replayDataArray[i].result);
    }
}

// Function to handle row clicks and determine the selected row and ID
void handleRowClick() {
    if (MousePos.Y >= TABLE_START_Y && MousePos.Y < TABLE_START_Y + (MAX_REPLAYS * ROW_HEIGHT)) {
        int clickedRow = (MousePos.Y - TABLE_START_Y) / ROW_HEIGHT;
        if (clickedRow < MAX_REPLAYS) {
            replayId = replayDataArray[clickedRow].id;
        }
    }
}

void handleOnScreenReplayManagement() {
    if (MousePos.Y == (TABLE_START_Y + 3 + MAX_REPLAYS * ROW_HEIGHT + BUTTON_Y_OFFSET) && MousePos.X >= 16 && MousePos.X <= 22) {
        // Example: Call delete function (You can change to other operations)
        deleteReplay(replayId);
    } else if (MousePos.Y == (TABLE_START_Y + 3 + MAX_REPLAYS * ROW_HEIGHT + BUTTON_Y_OFFSET) && MousePos.X >= 30 && MousePos.X <= 36) {
        // Free memory when no longer needed
        free(replayDataArray);
        frameAdminScreen();
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
        // Call function to send data to server here
        // deleteGame(replayId);
        adminDeleteReplay(sockfd, replayId);
    }
}
