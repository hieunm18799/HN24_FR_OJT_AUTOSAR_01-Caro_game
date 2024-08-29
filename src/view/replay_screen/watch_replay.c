#include <stdio.h>
#include <Windows.h>

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
#define START_POSITION_Y (console_height - 3)
#define STOP_POSITION_X 20
#define STOP_POSITION_Y (console_height - 3)
#define BACK_POSITION_X 5
#define BACK_POSITION_Y (console_height - 1) 
#define board_width 10
#define board_height 10
#define BUTTON_WIDTH 10
int console_width, console_height;
int replay_active = 0; // Flag to track replay state
int current_move_index = 0;

// void gotoxy(int x, int y) {
//     COORD coord;
//     coord.X = x;
//     coord.Y = y;
//     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
// }

void handleStartStop() {
    replay_active = !replay_active;  // Toggle replay state
}

void set_console_size(int width, int height) {
    COORD buffer_size = { width, height };
    SMALL_RECT window_size = { 0, 0, width - 1, height - 1 };
    
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), buffer_size);
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &window_size);
}


COORD MousePos;
int Click_flag;
int currentScreen;

// Hàm xử lý sự kiện click chuột
void handleMouseClick() {
    HANDLE hInput;
    DWORD events;
    INPUT_RECORD inputRecord;

    hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

    // Đọc sự kiện chuột
    if (ReadConsoleInput(hInput, &inputRecord, 1, &events)) {
        if (inputRecord.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD mouseEvent = inputRecord.Event.MouseEvent;
            if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                MousePos = mouseEvent.dwMousePosition; // Lưu vị trí chuột
                Click_flag = 1;  // Đặt cờ click để chỉ ra rằng đã có sự kiện click
                Sleep(100); // Tránh đọc nhiều lần cùng một sự kiện click
            }
        }
    }
}

void DrawReplayBoard() {
    system("cls");
    currentScreen = VIEW_WATCH_REPLAY;

    console_width = board_width * 4 + 5;
    console_height = board_height * 2 + 10;

    // Set the console size
    set_console_size(console_width, console_height);

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
    if (Click_flag == 1) {
        Click_flag = 0;
        if (MousePos.Y == START_POSITION_Y && MousePos.X >= START_POSITION_X && MousePos.X <= (START_POSITION_X + BUTTON_WIDTH)) {
            replay_active = 1; // Bắt đầu replay
        } else if (MousePos.Y == STOP_POSITION_Y && MousePos.X >= STOP_POSITION_X && MousePos.X <= (STOP_POSITION_X + BUTTON_WIDTH)) {
            replay_active = 0; // Dừng replay
        } else if (MousePos.Y == BACK_POSITION_Y && MousePos.X >= BACK_POSITION_X && MousePos.X <= (BACK_POSITION_X + BUTTON_WIDTH)) {
            currentScreen = VIEW_REPLAY_LIST; // Trở về màn hình trước
            system("cls");    
        }
    }
}

void ReplayGameInfo(int move[], int move_count) {
    if (current_move_index >= move_count) {
        replay_active = 0;
        current_move_index = 0;
        return;
    }

    int move_x = move[current_move_index];
    int move_y = move[current_move_index + 1];

    // Di chuyển con trỏ đến tọa độ cần vẽ
    gotoxy(CARO_BOARD_POSITION_X + move_x * 4 + 2, CARO_BOARD_POSITION_Y + move_y * 2 + 1);

    // Kiểm tra nước đi của người chơi (Player 1 hoặc Player 2)
    if (current_move_index % 4 == 0) {
        printf("X");
    } else {
        printf("O");
    }

    current_move_index += 2;

    // Khoảng dừng dài hơn và kiểm tra thường xuyên
    for (int i = 0; i < 1000; i += 100) {
        if (!replay_active) break;  // Kiểm tra nút "Stop" thường xuyên
        Sleep(100);  // Dừng trong 100ms và sau đó kiểm tra lại
        handleMouseClick();  // Đảm bảo có thể bấm nút trong lúc chờ
    }
}


void handleReplayButton(int move[], int move_count) {
    if (replay_active) {
        ReplayGameInfo(move, move_count);
    }
}


// int main() {
//     DrawReplayBoard();
//     int moves[] = {1, 2, 3, 4, 2, 3, 4, 5,6,7,8,9,0,2};
//     int move_count = sizeof(moves) / sizeof(int);

//     while (1) {
//         handleMouseClick();
//         handleClickOnWatchReplayScreen();
//         handleReplayButton(moves, move_count);
//     }

//     return 0;
// }

