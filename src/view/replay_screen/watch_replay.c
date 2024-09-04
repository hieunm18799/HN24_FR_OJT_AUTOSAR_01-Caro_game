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
         replayDataArray = (ReplayData *)malloc(MAX_REPLAYS * sizeof(ReplayData));
		if (replayDataArray == NULL) {
			printf("Không thể cấp phát bộ nhớ\n");
			return;
		}

        // handleReplayButton(move_data[14], move_data_count);
		// Data fetching from server
        fetchReplayInfoData();
        
		// Draw the initial UI
		drawReplayInfoUI();

		// Display the fetched data
		displayReplayInfoData();
        }
    // }
}

void ReplayGameInfo(const MatchHistory *history) {
    if (history == NULL || history->moves == NULL) {
        printf("No match history available.\n");
        return;
    }

    // Hiển thị thông tin trận đấu
    printf("Player 1: %s\n", history->player1_name);
    printf("Player 2: %s\n", history->player2_name);
    printf("Game ID: %u\n", history->game_id);
    printf("Kết quả: %s\n", history->result);

    // Vẽ bảng cờ trước khi phát lại các nước đi
    DrawReplayBoard();

    int moves_replayed = 0;
    Move *current_move = history->moves;

    // Duyệt qua danh sách liên kết để phát lại các nước đi
    while (current_move != NULL) {
        // Di chuyển con trỏ đến vị trí vẽ
        gotoxy(CARO_BOARD_POSITION_X + current_move->x * 4 + 2, CARO_BOARD_POSITION_Y + current_move->y * 2 + 1);

        // Xác định người chơi và in X hoặc O
        if (moves_replayed % 2 == 0) { // Nước đi chẵn: Người chơi 1
            printf("X");
        } else { // Nước đi lẻ: Người chơi 2
            printf("O");
        }

        // Cập nhật lại mảng `board[][]` với nước đi mới
        board[current_move->y][current_move->x] = (moves_replayed % 2 == 0) ? 'X' : 'O';

        moves_replayed++;
        current_move = current_move->next;

        // Tạm dừng giữa các nước đi
        Sleep(1000);  // Pause for 1 second
    }
    printf("\nReplay completed. Press any key to exit...");
    getchar(); // Đọc ký tự từ bàn phím để dừng chương trình
}

