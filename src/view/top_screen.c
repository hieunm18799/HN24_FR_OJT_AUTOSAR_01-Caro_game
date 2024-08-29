#include <windows.h>
#include <stdio.h>
#include <string.h>

#define USERNAME_MAX_LEN 20

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void drawLoginScreen() {
    system("cls");  // Clear the screen
    gotoxy(10, 5);
    printf("Username: ");
    gotoxy(10, 7);
    printf("[ Login ]");
}

int main() {
    char username[USERNAME_MAX_LEN + 1] = {0};
    int cursorPos = 0;
    HANDLE hInput;
    INPUT_RECORD inputRecord;
    DWORD events;

    // Enable mouse input
    hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT);

    drawLoginScreen();
    gotoxy(20, 5);  // Move the cursor to the input position

    while (1) {
        ReadConsoleInput(hInput, &inputRecord, 1, &events);

        if (inputRecord.EventType == KEY_EVENT) {
            KEY_EVENT_RECORD keyEvent = inputRecord.Event.KeyEvent;
            if (keyEvent.bKeyDown) {
                if (keyEvent.wVirtualKeyCode == VK_RETURN) {
                    if (strlen(username) > 0) {
                        gotoxy(10, 9);
                        printf("Logging in as %s...\n", username);
                        break;
                    }
                } else if (keyEvent.wVirtualKeyCode == VK_BACK) {
                    if (cursorPos > 0) {
                        cursorPos--;
                        username[cursorPos] = '\0';
                        gotoxy(20 + cursorPos, 5);
                        printf(" ");
                        gotoxy(20 + cursorPos, 5);
                    }
                } else if (keyEvent.uChar.AsciiChar != 0 && cursorPos < USERNAME_MAX_LEN) {
                    username[cursorPos] = keyEvent.uChar.AsciiChar;
                    cursorPos++;
                    printf("%c", keyEvent.uChar.AsciiChar);
                }
            }
        } else if (inputRecord.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD mouseEvent = inputRecord.Event.MouseEvent;

            if (mouseEvent.dwEventFlags == 0 && mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                int mouseX = mouseEvent.dwMousePosition.X;
                int mouseY = mouseEvent.dwMousePosition.Y;

                // Debug output to show mouse position
                gotoxy(0, 10);
                printf("Mouse Clicked at (%d, %d)\n", mouseX, mouseY);

                if (mouseX >= 10 && mouseX <= 17 && mouseY == 7) {
                    if (strlen(username) > 0) {
                        gotoxy(10, 9);
                        printf("Logging in as %s...\n", username);
                        break;
                    } else {
                        gotoxy(10, 9);
                        printf("Username cannot be empty.\n");
                    }
                }
            }
        }
    }

    return 0;
}