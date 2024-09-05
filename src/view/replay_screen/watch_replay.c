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
#define BACK_POSITION_X 5
#define BACK_POSITION_Y (board_height * 2 + 9) 
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
    printf("<X> Player: %s - %d Win %d - Lose",player1_username, player1_win, player1_lose);

    gotoxy(PLAYER_2_POSITION_X, PLAYER_2_POSITION_Y);
    printf("<O> Player: %s - %d Win %d - Lose",player2_username, player2_win, player2_lose);

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

    gotoxy(START_POSITION_X, START_POSITION_Y);
    printf("[ START ]");

    gotoxy(STOP_POSITION_X, STOP_POSITION_Y);
    printf("[ STOP ]");

    gotoxy(BACK_POSITION_X, BACK_POSITION_Y);
    printf("[ BACK ]");
}


void handleClickOnWatchReplayScreen() {

        if (MousePos.Y == START_POSITION_Y && MousePos.X >= START_POSITION_X && MousePos.X <= (START_POSITION_X + BUTTON_WIDTH)) {
            replay_active = 1; // Bắt đầu replay

        } else if (MousePos.Y == STOP_POSITION_Y && MousePos.X >= STOP_POSITION_X && MousePos.X <= (STOP_POSITION_X + BUTTON_WIDTH)) {
            replay_active = 0; // Dừng replay

        } else if (MousePos.Y == BACK_POSITION_Y && MousePos.X >= BACK_POSITION_X && MousePos.X <= (BACK_POSITION_X + BUTTON_WIDTH)) {
         // Trở về màn hình SHOW REPLAY
        //  replayDataArray = (ReplayData *)malloc(MAX_REPLAYS * sizeof(ReplayData));
		// if (replayDataArray == NULL) {
		// 	printf("Không thể cấp phát bộ nhớ\n");
		// 	return;
		// }

        // // handleReplayButton(move_data[14], move_data_count);
		// // Data fetching from server
        // fetchReplayInfoData();
        
		// // Draw the initial UI
		// drawReplayInfoUI();

		// // Display the fetched data
		// displayReplayInfoData();
        }
    // }
}

void ReplayGameInfo(const char *move_data) {
    if (move_data == NULL || strlen(move_data) == 0) {
        printf("No move data available.\n");
        return;
    }

    int moves_replayed = 0;
    char moves_copy[MAXIMUM_SIZE];
    strncpy(moves_copy, move_data, MAXIMUM_SIZE - 1);
    moves_copy[MAXIMUM_SIZE - 1] = '\0';  // Ensure the string is null-terminated

    // Parse the move data using strtok
    char *move = strtok(moves_copy, "@");
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

    printf("\nReplay completed. Press any key to exit...");
    getchar();  // Wait for user input to end the replay
}
