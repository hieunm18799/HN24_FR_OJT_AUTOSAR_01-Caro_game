#include "top_screen.h"
#include "games.h"

// Macro definitions for UI layout
#define TITLE_X 27
#define TITLE_Y 3
#define TABLE_START_X 0
#define TABLE_START_Y 8
#define TABLE_ROW_HEIGHT 2
#define TABLE_WIDTH 52
#define BUTTON_WATCH_X 16
#define BUTTON_BACK_X 30
#define BUTTON_Y_OFFSET 2
#define BUTTON_WIDTH 6
#define ID_COLUMN 3
#define PLAYER1_COLUMN 13
#define PLAYER2_COLUMN 28
#define RESULT_COLUMN 43

void drawReplayInfoUI() {
    system("cls");

    // Draw title
    gotoxy(TITLE_X, TITLE_Y);
    printf("Replay Information");

    // Draw table header
    gotoxy(TABLE_START_X, TABLE_START_Y);
    printf("----------------------------------------------------");

    gotoxy(1, TABLE_START_Y + 1);
    printf("|   id   |   player1   |   player2   |   result   |");

    gotoxy(TABLE_START_X, TABLE_START_Y + 2);
    printf("----------------------------------------------------");

    // Draw table rows
    for (int i = 0; i < MAX_REPLAYS; i++) {
        gotoxy(1, TABLE_START_Y + 3 + i * TABLE_ROW_HEIGHT);
        printf("|        |             |             |            |");
    }

    // Draw table footer
    gotoxy(TABLE_START_X, TABLE_START_Y + 3 + MAX_REPLAYS * TABLE_ROW_HEIGHT);
    printf("----------------------------------------------------");

    // Draw buttons
    gotoxy(BUTTON_WATCH_X, TABLE_START_Y + 3 + MAX_REPLAYS * TABLE_ROW_HEIGHT + BUTTON_Y_OFFSET);
    printf("[ Watch ]");

    gotoxy(BUTTON_BACK_X, TABLE_START_Y + 3 + MAX_REPLAYS * TABLE_ROW_HEIGHT + BUTTON_Y_OFFSET);
    printf("[ Back ]");

    // Set the current screen
    currentScreen = VIEW_REPLAY_LIST;
}

void displayReplayInfoData() {
    for (int i = 0; i < MAX_REPLAYS; i++) {
        gotoxy(ID_COLUMN, TABLE_START_Y + 3 + i * TABLE_ROW_HEIGHT);
        printf("%02d", replayDataArray[i].id);

        gotoxy(PLAYER1_COLUMN, TABLE_START_Y + 3 + i * TABLE_ROW_HEIGHT);
        printf("%-9s", replayDataArray[i].player1);

        gotoxy(PLAYER2_COLUMN, TABLE_START_Y + 3 + i * TABLE_ROW_HEIGHT);
        printf("%-9s", replayDataArray[i].player2);

        gotoxy(RESULT_COLUMN, TABLE_START_Y + 3 + i * TABLE_ROW_HEIGHT);
        printf("%-7s", replayDataArray[i].result);
    }
}

void handleOnScreenReplayInfo() {
    int buttonY = TABLE_START_Y + 3 + MAX_REPLAYS * TABLE_ROW_HEIGHT + BUTTON_Y_OFFSET;

    // Handle "Watch" button click
    if (MousePos.Y == buttonY && MousePos.X >= BUTTON_WATCH_X && MousePos.X <= (BUTTON_WATCH_X + BUTTON_WIDTH)) {
        DrawReplayBoard();
        // Implement returning replayId to control
    }
    // Handle "Back" button click
    else if (MousePos.Y == buttonY && MousePos.X >= BUTTON_BACK_X && MousePos.X <= (BUTTON_BACK_X + BUTTON_WIDTH)) {
        free(replayDataArray);
        dashboard();
    }
}


void fetchReplayInfoData() {
    // Simulate fetching replay data (example data)
    for (int i = 0; i < MAX_REPLAYS; i++) {
        replayDataArray[i].id = i + 1;
        sprintf(replayDataArray[i].player1, "Player %d", i + 1);
        sprintf(replayDataArray[i].player2, "Player %d", MAX_REPLAYS - i);
        sprintf(replayDataArray[i].result, i % 2 == 0 ? "Win" : "Loss");
    }
}

void addReplayData(unsigned int game_id, char *player1, char *player2, char *result) {
    if (player1 == NULL || player2 == NULL || result == NULL) return;
    replayDataArray = (ReplayData*)realloc(sizeof(ReplayData), MAX_REPLAYS + 1);
    if (replayDataArray == NULL) {
        printf("Không thể cấp phát bộ nhớ\n");
        return;
    }
    replayDataArray[MAX_REPLAYS].id = game_id;
    strcpy(replayDataArray[MAX_REPLAYS].player1, player1);
    strcpy(replayDataArray[MAX_REPLAYS].player2, player2);
    strcpy(replayDataArray[MAX_REPLAYS].result, result);
    MAX_REPLAYS++;
}
