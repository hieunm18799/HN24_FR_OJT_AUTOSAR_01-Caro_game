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
    printf("<X> PLAYER 1");

    gotoxy(PLAYER_2_POSITION_X, PLAYER_2_POSITION_Y);
    printf("<O> PLAYER 2");

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
         replayDataArray = (ReplayData *)malloc(MAX_REPLAYS * sizeof(ReplayData));
		if (replayDataArray == NULL) {
			printf("Không thể cấp phát bộ nhớ\n");
			return;
		}

		// Draw the initial UI
		drawReplayInfoUI();

		// Display the fetched data
		displayReplayInfoData();
        }
    // }
}


void ReplayGameInfo(int board[MAXIMUM_SIZE][MAXIMUM_SIZE]) {

    // Iterate over the board to replay the moves
    for (int cell_x = 0; cell_x < board_width; ++cell_x) {
        for (int cell_y = 0; cell_y < board_height; ++cell_y) {
            if (board[cell_x][cell_y] != 0) {  // A move has been made at this position
                // Move cursor to the drawing position
                gotoxy(CARO_BOARD_POSITION_X + cell_x * 4 + 2, CARO_BOARD_POSITION_Y + cell_y * 2 + 1);

                // Check the player and print X or O
                if (board[cell_x][cell_y] == 'X') {
                    printf("X");
                } else if (board[cell_x][cell_y] == 'O') {
                    printf("O");
                }

            }
        }
    }

}
