#include "top_screen.h"

#define CARO_GAME_STRING_POSITION_X 10
#define CARO_GAME_STRING_POSITION_Y 0
#define PLAYER_1_POSITION_X 5
#define PLAYER_1_POSITION_Y 2
#define PLAYER_2_POSITION_X 5
#define PLAYER_2_POSITION_Y 3
#define CARO_BOARD_POSITION_X 0
#define CARO_BOARD_POSITION_Y 5
#define MAXIMUM_SIZE 100
#define START_POSITION_X 5
#define START_POSITION_Y (board_height * 2 + 7)
#define STOP_POSITION_X 20
#define STOP_POSITION_Y (board_height * 2 + 7)
#define BACK_POSITION_X 25
#define BACK_POSITION_Y 2
#define board_width 10
#define board_height 10
#define BUTTON_WIDTH 10
int console_width, console_height;
int replay_active = 0; // Flag to track replay state
int current_move_index = 0;
char board[MAXIMUM_SIZE][MAXIMUM_SIZE]; 


void DrawReplayBoard() {
    system("cls");
    currentScreen = VIEW_WATCH_REPLAY;

    console_width = board_width * 4 + 5;
    console_height = board_height * 2 + 10;

    gotoxy(CARO_GAME_STRING_POSITION_X, CARO_GAME_STRING_POSITION_Y);
    printf("CARO GAME");

    gotoxy(PLAYER_1_POSITION_X, PLAYER_1_POSITION_Y);
    printf("<X> Player 1");

    gotoxy(PLAYER_2_POSITION_X, PLAYER_2_POSITION_Y);
    printf("<O> Player 2");

    gotoxy(CARO_BOARD_POSITION_X, CARO_BOARD_POSITION_Y);
    for (int i = 0; i < board_height * 2 + 1; i++) {
        if (i % 2 == 0) {
            for (int j = 0; j < board_width * 4 + 1; j++) {
                if (j % 4 == 0) printf("+");
                else printf("-");
            }
        } else {
            for (int j = 0; j < board_width * 4 + 1; j++) {
                if (j % 4 == 0) printf("|");
                else printf(" ");
            }
        }
        printf("\n");
    }

    gotoxy(BACK_POSITION_X, BACK_POSITION_Y);
    printf("[ BACK ]");
}


// Function to draw the game board with dynamic dimensions
void DrawDynamicReplayBoard(int width, int height) {
    system("cls");  // Clear the console screen

    gotoxy(CARO_GAME_STRING_POSITION_X, CARO_GAME_STRING_POSITION_Y);
    printf("CARO GAME");

    gotoxy(PLAYER_1_POSITION_X, PLAYER_1_POSITION_Y);
    printf("<X> Player 1");

    gotoxy(PLAYER_2_POSITION_X, PLAYER_2_POSITION_Y);
    printf("<O> Player 2");

    gotoxy(CARO_BOARD_POSITION_X, CARO_BOARD_POSITION_Y);
    for (int i = 0; i < height * 2 + 1; i++) {
        if (i % 2 == 0) {
            for (int j = 0; j < width * 4 + 1; j++) {
                if (j % 4 == 0) printf("+");
                else printf("-");
            }
        } else {
            for (int j = 0; j < width * 4 + 1; j++) {
                if (j % 4 == 0) printf("|");
                else printf(" ");
            }
        }
        printf("\n");
    }
    gotoxy(BACK_POSITION_X, BACK_POSITION_Y);
    printf("[ BACK ]");
}

void handleClickOnWatchReplayScreen() {

        if (MousePos.Y == START_POSITION_Y && MousePos.X >= START_POSITION_X && MousePos.X <= (START_POSITION_X + BUTTON_WIDTH)) {
            replay_active = 1; // Bắt đầu replay

        } else if (MousePos.Y == STOP_POSITION_Y && MousePos.X >= STOP_POSITION_X && MousePos.X <= (STOP_POSITION_X + BUTTON_WIDTH)) {
            replay_active = 0; // Dừng replay

        } else if (MousePos.Y == BACK_POSITION_Y && MousePos.X >= BACK_POSITION_X && MousePos.X <= (BACK_POSITION_X + BUTTON_WIDTH)) {
            drawReplayInfoUI();
            MAX_REPLAYS = 0;
            replayDataArray = NULL;
            getUsernameReplaysData(sockfd, signed_in_username);
        }
    // }
}

void ReplayGameInfo(const char *move_data) {
    if (move_data == NULL || strlen(move_data) == 0) {
        printf("No move data available.\n");
        return;
    }

    int max_x = board_width;  // Initial maximum width of the board
    int max_y = board_height; // Initial maximum height of the board
    char moves_copy[MAXIMUM_SIZE];
    strncpy(moves_copy, move_data, MAXIMUM_SIZE - 1);
    moves_copy[MAXIMUM_SIZE - 1] = '\0';  // Ensure the string is null-terminated

    // First pass: Check for the maximum x and y coordinates in the move data
    char *move = strtok(moves_copy, "@");
    while (move != NULL) {
        int x, y;
        if (sscanf(move, "%d-%d", &x, &y) == 2) {
            // Update the maximum dimensions based on the move coordinates
            if (x >= max_x) max_x = x + 1;
            if (y >= max_y) max_y = y + 1;
        }
        move = strtok(NULL, "@");
    }

    // Calculate the required console dimensions
    int console_width = max_x * 4 + 10;  // Board width + extra space
    int console_height = max_y * 2 + 15; // Board height + UI space

    // Set the console size dynamically
    SetConsoleSize(console_width, console_height);    

    // Re-draw the board based on the largest detected move coordinates
    DrawDynamicReplayBoard(max_x, max_y);

    // Second pass: Replay the moves
    strncpy(moves_copy, move_data, MAXIMUM_SIZE - 1);  // Reset the move data string
    moves_copy[MAXIMUM_SIZE - 1] = '\0';  // Ensure null termination
    move = strtok(moves_copy, "@");  // Restart parsing the moves
    int moves_replayed = 0;

    while (move != NULL) {
        int x, y;
        if (sscanf(move, "%d-%d", &x, &y) == 2) {
            // Draw the current move on the board
            gotoxy(CARO_BOARD_POSITION_X + x * 4 + 2, CARO_BOARD_POSITION_Y + y * 2 + 1);
            if (moves_replayed % 2 == 0) {
                printf("X");  // Player 1 move
            } else {
                printf("O");  // Player 2 move
            }

            // Update the board array
            board[y][x] = (moves_replayed % 2 == 0) ? 'X' : 'O';

            moves_replayed++;
        } else {
            printf("Invalid move format: %s\n", move);
        }

        move = strtok(NULL, "@");  // Move to the next part of the string

        // Pause between moves
        Sleep(1000);  // 1-second delay between moves
    }

    gotoxy(PLAYER_2_POSITION_X + 20, PLAYER_2_POSITION_Y);
    printf("Replay completed.");
}

