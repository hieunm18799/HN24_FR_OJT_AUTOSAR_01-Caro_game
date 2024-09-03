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

void handleStartStop() {
    replay_active = !replay_active;  // Toggle replay state
}

// void set_console_size(int width, int height) {
//     COORD buffer_size = { width, height };
//     SMALL_RECT window_size = { 0, 0, width - 1, height - 1 };
    
//     SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), buffer_size);
//     SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &window_size);
// }

void DrawReplayBoard() {
    system("cls");
    currentScreen = VIEW_WATCH_REPLAY;

    console_width = board_width * 4 + 5;
    console_height = board_height * 2 + 10;

    // Set the console size
    // set_console_size(console_width, console_height);

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
    // if (Click_flag == 1) {
    //     Click_flag = 0;
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

        // handleReplayButton(move_data[14], move_data_count);
		// Data fetching from server
        
		// Draw the initial UI
		drawReplayInfoUI();

		// Display the fetched data
		displayReplayInfoData();
        }
    // }
}


void ReplayGameInfo(int board[MAXIMUM_SIZE][MAXIMUM_SIZE]) {
    if (!replay_active) return;  // Early return if replay is not active

    int move_count = 0; //Tổng số bước đi trong replay

    // Calculate the total number of moves from the board
    for (int x = 0; x < MAXIMUM_SIZE; ++x) {
        for (int y = 0; y < MAXIMUM_SIZE; ++y) {
            if (board[x][y] != 0) {
                move_count++;
            }
        }
    }

    int moves_replayed = 0; // Đếm số bước đã đi bắt đầu từ lúc replay.

    // Iterate over the board to replay the moves
    for (int cell_x = 0; cell_x < MAXIMUM_SIZE && moves_replayed < move_count; ++cell_x) {
        for (int cell_y = 0; cell_y < MAXIMUM_SIZE && moves_replayed < move_count; ++cell_y) {
            if (board[cell_x][cell_y] != 0) {  // A move has been made at this position
                // Move cursor to the drawing position
                gotoxy(CARO_BOARD_POSITION_X + cell_x * 4 + 2, CARO_BOARD_POSITION_Y + cell_y * 2 + 1);

                // Check the player and print X or O
                if (board[cell_x][cell_y] == 1) {
                    printf("X");
                } else if (board[cell_x][cell_y] == 2) {
                    printf("O");
                }

                moves_replayed++;

                // Pause briefly between moves
                for (int i = 0; i < 10; i++) {
                    if (!replay_active) return;  // Stop if replay_active is set to 0
                    Sleep(50);  // Sleep for a short period to allow for frequent checks
                    handleMouseClick();  // Check for mouse click events
                }
            }
        }
    }

    // When the replay finishes, reset the state
    replay_active = 0;
}

// int main() {
//     // Initialize a test board with some moves
//     int test_board[BOARD_SIZE][BOARD_SIZE] = {
//         {1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
//         {0, 0, 1, 2, 0, 0, 0, 0, 0, 0},
//         {0, 0, 0, 0, 1, 2, 0, 0, 0, 0},
//         {0, 0, 0, 0, 0, 0, 1, 2, 0, 0},
//         {0, 0, 0, 0, 0, 0, 0, 0, 1, 2},
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
//     };

//     DrawReplayBoard();

//     while (1) {
//         handleMouseClick();
//         handleClickOnWatchReplayScreen();
//         ReplayGameInfo(test_board);  // Now it checks replay_active inside the function
//     }

//     return 0;
// }