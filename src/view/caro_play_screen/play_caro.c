#include "top_screen.h"

#define CARO_GAME_STRING_POSITION_X 10
#define CARO_GAME_STRING_POSITION_Y 0
#define PLAYER_1_POSITION_X 5
#define PLAYER_1_POSITION_Y 2
#define PLAYER_2_POSITION_X 5
#define PLAYER_2_POSITION_Y 3
#define CARO_BOARD_POSITION_X 0
#define CARO_BOARD_POSITION_Y 9
#define MAXIMUM_SIZE 100
#define REDO_POSITION_X 5
#define REDO_POSITION_Y 5     
// #define COUNT_DOWN_POSITION_X 5
// #define COUNT_DOWN_POSITION_Y (board_height * 2 + 8)
#define QUIT_POSITION_X 5
#define QUIT_POSITION_Y 7
#define BUTTON_WIDTH 5  // Approximate width of the buttons
#define BUTTON_HEIGHT 1  // Height of the buttons
#define CELL_WIDTH 4
#define CELL_HEIGHT 2
#define COUNT_DOWN_TIME 10
#define COUNT_DOWN_PRINT_TURN 13
#define AGREE_POSITION_X 15
#define AGREE_POSITION_Y 5
#define WIN_CONDITION 4
#define WIN_NOTIFY 15

// Function prototypes
// void SetConsoleSize(int width, int height);
void drawPlayCaroBoard();
int MovePlayCaro();
// void DisplayCountdown();
void handleRedoRequest();
// int CheckWin(int last_x, int last_y);
void printMessagePlayCaro(const char *format, ...);
void RedrawPlayCaroBoard ();

// Global variables
static COORD CursorPosition;
static int Player1_turn = 1;  // Player 1 starts first
static int End_flag = 1;      // Flag to control the display loop
// static int countdown_time = COUNT_DOWN_TIME;
// static int countdown_active = 1;
static int redo_requested = 0; // Flag to indicate if redo is requested
static int redo_agreed = 0;    // Flag to indicate if redo is agreed

char move_sig;
int currentScreen;
int board_width = 10, board_height = 10;

int console_width, console_height, new_console_width, new_console_height; // Console dimensions
char board[MAXIMUM_SIZE][MAXIMUM_SIZE]; // Maximum board size 100x100

int last_move_x = -1;
int last_move_y = -1;

// DWORD WINAPI CountdownThread(LPVOID lpParam) {
//     DisplayCountdown();
//     return 0;
// }

HANDLE hThread;

// Function to set the console size
// void SetConsoleSize(int width, int height) {
//     COORD buffer_size = { width, height };
//     SMALL_RECT window_size = { 0, 0, width - 1, height - 1 };
    
//     SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), buffer_size);
//     SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &window_size);
// }

// Function to draw the board and initialize board state
void drawPlayCaroBoard() {
    if (strcmp(signed_in_username, player1_username) == 0) move_sig = 'X';
    else move_sig = 'O';

    End_flag = 1;
    Player1_turn = 1;
    // countdown_time = COUNT_DOWN_TIME;
    // countdown_active = 1;
    redo_requested = 0;
    redo_agreed = 0;
    last_move_x = -1;
    last_move_y = -1;
    
    system("cls");

    currentScreen = VIEW_PLAY_GAME;
    // Input board size from the user
    // printf("Enter board width (number of cells): ");
    // scanf("%d", &board_width);
    // printf("Enter board height (number of cells): ");
    // scanf("%d", &board_height);
    system("cls");

    // // Initialize the board state
    // for (int i = 0; i < board_height; i++) {
    //     for (int j = 0; j < board_width; j++) {
    //         board[i][j] = ' '; // Empty cell
    //     }
    // }

    // Calculate console size based on board dimensions
    // console_width = board_width * 4 + 5;
    // console_height = board_height * 2 + 10;

    // Set the console size
    //SetConsoleSize(console_width, console_height);

    // Set up game title position
    gotoxy(CARO_GAME_STRING_POSITION_X,CARO_GAME_STRING_POSITION_Y);
    printf("CARO GAME");

    // Set up player 1 label position
    gotoxy(PLAYER_1_POSITION_X,PLAYER_1_POSITION_Y);
    printf("<X> Player: %s - %d Win %d - Lose",player1_username, player1_win, player1_lose);

    // Set up player 2 label position
    gotoxy(PLAYER_2_POSITION_X,PLAYER_2_POSITION_Y);
    printf("<X> Player: %s - %d Win - %d Lose",player2_username, player2_win, player2_lose);

    // Set up board drawing position
    gotoxy(CARO_BOARD_POSITION_X,CARO_BOARD_POSITION_Y);

    // Draw the board
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

    // Draw buttons
    gotoxy(REDO_POSITION_X,REDO_POSITION_Y);
    printf("[REDO]");

    // gotoxy(COUNT_DOWN_POSITION_X,COUNT_DOWN_POSITION_Y);
    // printf("COUNTDOWN:");

    gotoxy(AGREE_POSITION_X,AGREE_POSITION_Y);
    printf("[AGREE]");

    gotoxy(QUIT_POSITION_X,QUIT_POSITION_Y);
    printf("[QUIT]");

    // hThread = CreateThread(NULL, 0, CountdownThread, NULL, 0, NULL);
    // if (hThread == NULL) {
    //     printf("Error creating thread\n");
    //     return;
    // }
}

// void DisplayCountdown() {
//     while (End_flag == 1) {
//         if (countdown_active) {
//             gotoxy(COUNT_DOWN_POSITION_X,COUNT_DOWN_POSITION_Y);
//             printf("COUNTDOWN:");
//             if (Player1_turn) {
//                 gotoxy(COUNT_DOWN_POSITION_X + COUNT_DOWN_PRINT_TURN,COUNT_DOWN_POSITION_Y);
//                 printf("Player 1's turn: %2d", countdown_time);
//             } else {
//                 gotoxy(COUNT_DOWN_POSITION_X + COUNT_DOWN_PRINT_TURN,COUNT_DOWN_POSITION_Y);
//                 printf("Player 2's turn: %2d", countdown_time);
//             }

//             Sleep(1000); // Wait for 1 second
//             countdown_time--;

//             // Reset the countdown when it reaches 0
//             if (countdown_time < 0) {
//                 countdown_time = COUNT_DOWN_TIME;
//                 Player1_turn = !Player1_turn; // Switch turns
//             }
//         }
//     }
// }

int MovePlayCaro() {  // Change return type to int
    int cell_width = CELL_WIDTH;
    int cell_height = CELL_HEIGHT;

    // if (Click_flag == 1) {  // Process click if the flag is set
    //     Click_flag = 0;  // Reset flag to prevent continuous detection
        // countdown_active = 0; // Pause the countdown

        // Check if click is within "REDO" button
        if (MousePos.Y == REDO_POSITION_Y && MousePos.X >= REDO_POSITION_X && MousePos.X <= REDO_POSITION_X + BUTTON_WIDTH) {
            redo_requested = 1; // Set redo requested flag
            redoAsk(sockfd, signin_username, game_id);

            return 0;  // Indicate that no move was made during the redo request
        }

        // Check if click is within "QUIT" button
        if (MousePos.Y == QUIT_POSITION_Y && MousePos.X >= QUIT_POSITION_X && MousePos.X <= QUIT_POSITION_X + BUTTON_WIDTH) {
            End_flag = 0; // Exit the loop to quit the game
            // CloseHandle(hThread); // Clean up the thread handle
            //Redirect to TOP (LOGINED)
            // dashboard();
            quit(sockfd, game_id, signed_in_username);
            return 0;  // Indicate that no move was made due to quitting
        }

        // Check if click is within "AGREE" button
        if (MousePos.Y == AGREE_POSITION_Y && MousePos.X >= AGREE_POSITION_X && MousePos.X <= AGREE_POSITION_X + BUTTON_WIDTH) {
            redoAgree(sockfd, signin_username, game_id);
            return 0;  // Indicate that no move was made
        }

        // Calculate which cell was clicked based on the mouse position
        int cell_x = (MousePos.X - CARO_BOARD_POSITION_X) / cell_width;
        int cell_y = (MousePos.Y - CARO_BOARD_POSITION_Y) / cell_height;

        // Ensure the click is within the board boundaries
        if (cell_x >= 0 && cell_x < board_width && cell_y >= 0 && cell_y < board_height) {
            // Redraw the specific cell

            // if (board[cell_y][cell_x] == ' ') { // Check if the cell is empty
                // if (Player1_turn) {
                //     printf("X");
                //     board[cell_y][cell_x] = 'X'; // Update the board state
                //     Player1_turn = 0;
                // } else {
                //     printf("O");
                //     board[cell_y][cell_x] = 'O'; // Update the board state
                //     Player1_turn = 1;
                // }
                // Save the last move position
                // last_move_x = cell_x;
                // last_move_y = cell_y;

                //if (0) {
                //  if (CheckWin(cell_x, cell_y)) {
                //     gotoxy(PLAYER_1_POSITION_X + WIN_NOTIFY, PLAYER_1_POSITION_Y - 1);
                //     printf("Player %s wins!\n", Player1_turn ? "2" : "1");
                //     End_flag = 0; // End the game
                //     getchar();

                //     //Direct to TOP (LOGINED)
                //     dashboard();
                // } else 
                pick(sockfd, game_id, signed_in_username, cell_x, cell_y);

                // countdown_time = COUNT_DOWN_TIME;  // Reset the countdown for the next turn
                // countdown_active = 1;
                return 1;  // Indicate that a move was successfully made
            // }
        }
    //}

    return 0;  // Indicate that no move was made
}

void addPicked(char *username, unsigned char cell_x, unsigned char cell_y) {
    char temp = strcmp(username, signed_in_username) == 0 ? move_sig : (move_sig == 'X' ? 'O' : 'X');
    gotoxy(CARO_BOARD_POSITION_X + cell_x * CELL_WIDTH + 2, CARO_BOARD_POSITION_Y + cell_y * CELL_HEIGHT + 1);
    printf("%c", temp);
    last_move_x = cell_x;
    last_move_y = cell_y;
    // board[cell_y][cell_x] = temp; // Update the board state
    // if (CheckWin(cell_x, cell_y)) {
    //     printMessagePlayCaro("Player %s won!", username);
    //     End_flag = 0; // End the game
    //     //Direct to TOP (LOGINED)
    //     // dashboard();
    // }
}

void printMessagePlayCaro(const char *format, ...) {
    gotoxy(CARO_BOARD_POSITION_X, CARO_BOARD_POSITION_Y + board_height * 2 + 1);
    CursorPosition.X = 0;
    CursorPosition.Y = AGREE_POSITION_Y - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int console_width = csbi.dwSize.X;
    FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', console_width, CursorPosition, &written);
    va_list args;

    // Initialize the va_list with the variable arguments
    va_start(args, format);

    // Use vprintf to handle the variable arguments
    vprintf(format, args);

    // Clean up the va_list
    va_end(args);
}

void redoLastPicked(unsigned char last_move_x, unsigned char last_move_y) {
    gotoxy(CARO_BOARD_POSITION_X + last_move_x * CELL_WIDTH + 2,CARO_BOARD_POSITION_Y + last_move_y * CELL_HEIGHT + 1 );
    printf(" "); // Clear the last move from the board
    // board[last_move_y][last_move_x] = ' '; // Reset the board state
    printMessagePlayCaro("");
}

int CheckWin(int last_x, int last_y) {
    char player = board[last_y][last_x];
    int count;

    // Check horizontal
    count = 1;
    for (int i = last_x - 1; i >= 0 && board[last_y][i] == player; i--) count++;
    for (int i = last_x + 1; i < board_width && board[last_y][i] == player; i++) count++;
    if (count >= WIN_CONDITION) return 1;

    // Check vertical
    count = 1;
    for (int i = last_y - 1; i >= 0 && board[i][last_x] == player; i--) count++;
    for (int i = last_y + 1; i < board_height && board[i][last_x] == player; i++) count++;
    if (count >= WIN_CONDITION) return 1;

    // Check diagonal from top-left to bottom-right
    count = 1;
    for (int i = 1; last_x - i >= 0 && last_y - i >= 0 && board[last_y - i][last_x - i] == player; i++) count++;
    for (int i = 1; last_x + i < board_width && last_y + i < board_height && board[last_y + i][last_x + i] == player; i++) count++;
    if (count >= WIN_CONDITION) return 1;

    // Check diagonal from bottom-left to top-right
    count = 1;
    for (int i = 1; last_x - i >= 0 && last_y + i < board_height && board[last_y + i][last_x - i] == player; i++) count++;
    for (int i = 1; last_x + i < board_width && last_y - i >= 0 && board[last_y - i][last_x + i] == player; i++) count++;
    if (count >= WIN_CONDITION) return 1;

    return 0; // No win condition met
}

void RedrawPlayCaroBoard () {
    GetConsoleSize(&new_console_width, &new_console_height);
    board_width = (new_console_width - 5) / 4;
    board_height = (new_console_height - 10) / 2;

    //Redraw the board
    drawPlayCaroBoard();

    //Redraw the moves
    for (int cell_y = 0; cell_y < board_height; cell_y++) {
        for (int cell_x = 0; cell_x < board_width; cell_x++) {
            // Lấy ký tự từ mảng board
            char cell = board[cell_y][cell_x];

            // Nếu ô không rỗng, vẽ ký tự vào vị trí tương ứng
            if (cell != ' ') {
                gotoxy(CARO_BOARD_POSITION_X + cell_x * CELL_WIDTH + 2, CARO_BOARD_POSITION_Y + cell_y * CELL_HEIGHT + 1);
                printf("%c", cell);
            }
        }
    }   
}

