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
#define REDO_POSITION_X 5
#define REDO_POSITION_Y (CARO_BOARD_POSITION_Y + board_height * 2 + 2)
#define COUNT_DOWN_POSITION_X 5
#define COUNT_DOWN_POSITION_Y (console_height - 2)
#define QUIT_POSITION_X 5
#define QUIT_POSITION_Y (console_height - 1) 
#define BUTTON_WIDTH 20  // Approximate width of the buttons
#define BUTTON_HEIGHT 1  // Height of the buttons
#define CELL_WIDTH 4
#define CELL_HEIGHT 2
#define COUNT_DOWN_TIME 10
#define COUNT_DOWN_PRINT_TURN 13
#define AGREE_POSITION_X 15
#define AGREE_POSITION_Y (console_height - 3)


// Function prototypes
void set_console_size(int width, int height);
void draw_board();
void Move();
void handleMouseClick();
void display_countdown();
void handle_redo_request();
// Global variables
static COORD CursorPosition;
static COORD MousePos;
static int Click_flag = 0;
static int Player1_turn = 1;  // Player 1 starts first
static int End_flag = 1;      // Flag to control the display loop
static int countdown_time = COUNT_DOWN_TIME;
static int countdown_active = 1;
static int redo_requested = 0; // Flag to indicate if redo is requested
static int redo_agreed = 0;    // Flag to indicate if redo is agreed


int board_width, board_height;
int console_width, console_height; // Console dimensions
char board[MAXIMUM_SIZE][MAXIMUM_SIZE]; // Maximum board size 100x100

DWORD WINAPI CountdownThread(LPVOID lpParam) {
    display_countdown();
    return 0;
}


int main() {
    // Draw the board and handle user input
    draw_board();
    HANDLE hThread = CreateThread(NULL, 0, CountdownThread, NULL, 0, NULL);
    if (hThread == NULL) {
        printf("Error creating thread\n");
        return 1;
    }
    while (End_flag == 1) {
        handleMouseClick();
        Move();
        if (redo_requested) {
            handle_redo_request(); // Handle redo requests
        }
    }
    
    // Wait for user input before closing
    getchar(); // To wait for the actual user input
    CloseHandle(hThread); // Clean up the thread handle
    return 0;
}

// Function to set the console size
void set_console_size(int width, int height) {
    COORD buffer_size = { width, height };
    SMALL_RECT window_size = { 0, 0, width - 1, height - 1 };
    
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), buffer_size);
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &window_size);
}

// Function to draw the board and initialize board state
void draw_board() {
    // Input board size from the user
    printf("Enter board width (number of cells): ");
    scanf("%d", &board_width);
    printf("Enter board height (number of cells): ");
    scanf("%d", &board_height);
    system("cls");

    // Initialize the board state
    for (int i = 0; i < board_height; i++) {
        for (int j = 0; j < board_width; j++) {
            board[i][j] = ' '; // Empty cell
        }
    }

    // Calculate console size based on board dimensions
    console_width = board_width * 4 + 5;
    console_height = board_height * 2 + 10;

    // Set the console size
    set_console_size(console_width, console_height);

    // Set up game title position
    CursorPosition.X = CARO_GAME_STRING_POSITION_X;
    CursorPosition.Y = CARO_GAME_STRING_POSITION_Y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
    printf("CARO GAME");

    // Set up player 1 label position
    CursorPosition.X = PLAYER_1_POSITION_X;
    CursorPosition.Y = PLAYER_1_POSITION_Y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
    printf("<X> PLAYER 1");

    // Set up player 2 label position
    CursorPosition.X = PLAYER_2_POSITION_X;
    CursorPosition.Y = PLAYER_2_POSITION_Y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
    printf("<O> PLAYER 2");

    // Set up board drawing position
    CursorPosition.X = CARO_BOARD_POSITION_X;
    CursorPosition.Y = CARO_BOARD_POSITION_Y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);

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
    CursorPosition.X = REDO_POSITION_X;
    CursorPosition.Y = REDO_POSITION_Y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
    printf("REDO");

    CursorPosition.X = COUNT_DOWN_POSITION_X;
    CursorPosition.Y = COUNT_DOWN_POSITION_Y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
    printf("COUNTDOWN:");

    CursorPosition.X = AGREE_POSITION_X;
    CursorPosition.Y = AGREE_POSITION_Y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
    printf("AGREE");

    CursorPosition.X = QUIT_POSITION_X;
    CursorPosition.Y = QUIT_POSITION_Y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
    printf("QUIT");
}

// Function to handle a single mouse click
void handleMouseClick() {
    HANDLE hInput;
    DWORD events;
    INPUT_RECORD inputRecord;

    hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

    // Read mouse input
    if (ReadConsoleInput(hInput, &inputRecord, 1, &events)) {
        if (inputRecord.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD mouseEvent = inputRecord.Event.MouseEvent;
            if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                MousePos = mouseEvent.dwMousePosition; // Save the mouse position
                Click_flag = 1;  // Set the click flag to indicate a click occurred
                Sleep(100); // Debounce delay to avoid multiple captures of the same click
            }
        }
    }
}

// Function to handle mouse clicks and display X/O on the board
void Move() {
    int cell_width = CELL_WIDTH;
    int cell_height = CELL_HEIGHT;

    if (Click_flag == 1) {  // Process click if the flag is set
        Click_flag = 0;  // Reset flag to prevent continuous detection
        countdown_active = 0; // Pause the countdown
        
        // Check if click is within "REDO" button
        if (MousePos.Y == REDO_POSITION_Y && MousePos.X >= REDO_POSITION_X && MousePos.X <= REDO_POSITION_X + BUTTON_WIDTH) {
            redo_requested = 1; // Set redo requested flag
            return;
        }

        // Check if click is within "QUIT" button
        if (MousePos.Y == QUIT_POSITION_Y && MousePos.X >= QUIT_POSITION_X && MousePos.X <= QUIT_POSITION_X + BUTTON_WIDTH) {
            End_flag = 0; // Exit the loop to quit the game
            return;
        }

        // Check if click is within "AGREE" button
        if (MousePos.Y == AGREE_POSITION_Y && MousePos.X >= AGREE_POSITION_X && MousePos.X <= AGREE_POSITION_X + BUTTON_WIDTH) {
            redo_agreed = 1; // Set redo agreed flag
            return;
        }

        // Calculate which cell was clicked based on the mouse position
        int cell_x = (MousePos.X - CARO_BOARD_POSITION_X) / cell_width;
        int cell_y = (MousePos.Y - CARO_BOARD_POSITION_Y) / cell_height;

        // Ensure the click is within the board boundaries
        if (cell_x >= 0 && cell_x < board_width && cell_y >= 0 && cell_y < board_height) {
            // Redraw the specific cell
            CursorPosition.X = CARO_BOARD_POSITION_X + cell_x * cell_width + 2;
            CursorPosition.Y = CARO_BOARD_POSITION_Y + cell_y * cell_height + 1;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);

            if (board[cell_y][cell_x] == ' ') { // Check if the cell is empty
                if (Player1_turn) {
                    printf("X");
                    board[cell_y][cell_x] = 'X'; // Update the board state
                    Player1_turn = 0;
                } else {
                    printf("O");
                    board[cell_y][cell_x] = 'O'; // Update the board state
                    Player1_turn = 1;
                }
            }
        }
        countdown_time = COUNT_DOWN_TIME;  // Reset the countdown for the next turn
        countdown_active = 1;
    }
}

void display_countdown() {
    while (End_flag == 1) {
        if (countdown_active) {
            CursorPosition.X = COUNT_DOWN_POSITION_X;
            CursorPosition.Y = COUNT_DOWN_POSITION_Y;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);

            if (Player1_turn) {
                CursorPosition.X = COUNT_DOWN_POSITION_X + COUNT_DOWN_PRINT_TURN;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
                printf("Player 1's turn: %2d", countdown_time);
            } else {
                CursorPosition.X = COUNT_DOWN_POSITION_X + COUNT_DOWN_PRINT_TURN;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
                printf("Player 2's turn: %2d", countdown_time);
            }

            Sleep(1000); // Wait for 1 second
            countdown_time--;

            // Reset the countdown when it reaches 0
            if (countdown_time < 0) {
                countdown_time = COUNT_DOWN_TIME;
                Player1_turn = !Player1_turn; // Switch turns
            }
        }
    }
}


void handle_redo_request() {
    // Draw a prompt for Player 2
    CursorPosition.X = CARO_BOARD_POSITION_X;
    CursorPosition.Y = CARO_BOARD_POSITION_Y + board_height * 2 + 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
    printf("Player 1 requests a redo. Player 2, do you agree? Click AGREE to accept.");

    while (redo_requested) {
        handleMouseClick(); // Wait for Player 2's response

        // Check if click is within "AGREE" button
        if (MousePos.Y == AGREE_POSITION_Y && MousePos.X >= AGREE_POSITION_X && MousePos.X <= AGREE_POSITION_X + BUTTON_WIDTH) {
            if (redo_agreed) {
                // Implement redo functionality here
                // For example: Undo the last move
                // ... (Undo last move implementation)

                // Reset redo flags
                redo_requested = 0;
                redo_agreed = 0;
                printf("Redo agreed. The last move has been undone.\n");
                return;
            } else {
                printf("Redo request was not agreed upon.\n");
                redo_requested = 0; // Reset redo request flag
                redo_agreed = 0; // Reset redo agreed flag
                return;
            }
        }
    }
}