#include <stdio.h>
#include "games.h"
#include <windows.h>
#include "top_screen.h"

#define MAX_USERS 20
int userId = 0;
int selectedColumn;
int clickedRow;
char newData[50];

// Define structure to hold replay data
typedef struct {
    int id;
    char username[50];
    char password[50];
    char role[50];
    char win[50];
    char lose[50];
    char draw[50];
} userData;

// Example array and number of users
userData userDataArray[MAX_USERS]; 
int numUsers = 0; // Number of users

void frameUserManagement();
void addUserData();
void displayUserData();
int handleUserRowClick();
void handleOnScreenUserManagement();
void deleteUser(int userId);
void addUser();
//void sendDataToServer();
void editUser();
void enterData();

// Function to draw the replay management UI
void frameUserManagement() {
    system("cls");
    gotoxy(25, 2); printf("ADMIN DASHBOARD");
    gotoxy(20, 4); printf("[ Add ]");
    gotoxy(30, 4); printf("[ Edit ]");
    gotoxy(40, 4); printf("[ Delete ]");
    gotoxy(30, 6); printf("[  Back  ]");
    gotoxy(0, 7);
    printf("-------------------------------------------------------------------------------");
    gotoxy(0, 8);
    printf("|   STT   |   username   |     password     |   win   |   lose    |    draw    |");
    gotoxy(0, 9);
    printf("-------------------------------------------------------------------------------");

    for (int i = 0; i < 2*numUsers; i++) {
        gotoxy(0, tableStartY + i);
        printf("|        |              |                  |         |           |            |");
    }

    gotoxy(0, tableStartY + numUsers * rowHeight);
    printf("-------------------------------------------------------------------------------");
    currentScreen = VIEW_ADMIN_USER_MANAGE;
}


// Function to display replay data in the table
void displayUserData() {
    for (int i = 0; i < numUsers; i++) {
        gotoxy(4, tableStartY + i * rowHeight);
        printf("%02d", userDataArray[i].id);

        gotoxy(13, tableStartY + i * rowHeight);
        printf("%-9s", userDataArray[i].username);

        gotoxy(28, tableStartY + i  * rowHeight);
        printf("%-9s", userDataArray[i].password);

        gotoxy(48, tableStartY + i  * rowHeight);
        printf("%-9s", userDataArray[i].win);

        gotoxy(59, tableStartY + i  * rowHeight);
        printf("%-9s", userDataArray[i].lose);

        gotoxy(71, tableStartY + i  * rowHeight);
        printf("%-9s", userDataArray[i].draw);
    }
}

// Function to handle row clicks and determine the selected row and ID
int handleUserRowClick() {
    if (MousePos.Y >= tableStartY && MousePos.Y < tableStartY + (numUsers * rowHeight)) {
        clickedRow = (MousePos.Y - tableStartY) / rowHeight;
        if (clickedRow < numUsers) {
            userId = userDataArray[clickedRow].id;
            printf("Clicked on row %d with User ID: %d\n", clickedRow + 1, userId);
            
            if (MousePos.X >= 24 && MousePos.X < 43) {
                selectedColumn = 4;
            }
            else if (MousePos.X >= 43 && MousePos.X < 53) {
                selectedColumn = 1;
            }
            else if (MousePos.X >= 53 && MousePos.X < 65) {
                selectedColumn = 2;
            }
            else if (MousePos.X >= 63 && MousePos.X < 78) {
                selectedColumn = 3;
            }
        }
    }
    return clickedRow;
}

void handleOnScreenUserManagement() {
    if (MousePos.Y == 4 && MousePos.X >= 40 && MousePos.X <= 51) {
        deleteUser(userId);
    }
    else if (MousePos.Y == 4 && MousePos.X >= 20 && MousePos.X <= 29) {
        addUser();
    }
    else if (MousePos.Y == 4 && MousePos.X >= 30 && MousePos.X <= 39) {
        editUser();
    }
    else if (MousePos.Y == 6 && MousePos.X >= 30 && MousePos.X <= 41) {
        frameAdminScreen(); // Back
    }
}

//xoa 1 nguoi dung
//cac buoc xoa 1 nguoi dung
//1. chon hang co nguoi dung muon xoa
//2. nhan [delete]
void deleteUser(int userId) {
    int foundIndex = -1;

    // Step 1: Find the replay by its ID
    for (int i = 0; i < numUsers; i++) {
        if (userDataArray[i].id == userId) {
            foundIndex = i;
            break;
        }
    }

    adminDeleteUser(sockfd, userDataArray[foundIndex].username);

    // Step 2: If the replay is found, delete it
    if (foundIndex != -1) {
        // Step 3: Shift remaining replays to fill the gap
        for (int i = foundIndex; i < numUsers - 1; i++) {
            userDataArray[i] = userDataArray[i + 1];
        }

        // Step 4: Decrease the number of replays
        numUsers--;

        // Step 5: Clear the last replay (optional, for cleanliness)
        userDataArray[numUsers].id = 0;
        userDataArray[numUsers].username[0] = '\0';
        userDataArray[numUsers].password[0] = '\0';
        userDataArray[numUsers].win[0] = '\0';
        userDataArray[numUsers].lose[0] = '\0';
        userDataArray[numUsers].draw[0] = '\0';

        for (int i = 0; i < numUsers; i++) {
            userDataArray[i].id = i + 1;
        }

        // Step 6: Update the UI
        frameUserManagement();
        displayUserData();
    }
}

//them thong tin nguoi dung moi
//cac buoc them nguoi dung
//1. Nhan [Add]
//2. Nhap thong tin
//3. enter
void addUser() {
    int foundIndex;
    //add new row
    numUsers = numUsers + 1;
    userDataArray[numUsers - 1].id = numUsers;
    userDataArray[numUsers - 1].username[0] = '\0';
    userDataArray[numUsers - 1].password[0] = '\0';
    userDataArray[numUsers - 1].win[0] = '\0';
    userDataArray[numUsers - 1].lose[0] = '\0';
    userDataArray[numUsers - 1].draw[0] = '\0';

    // Update the UI
    frameUserManagement();
    displayUserData();

    //enter and save the data
    gotoxy(13, tableStartY + numUsers * rowHeight - 2);
    enterData();
    strcpy_s(userDataArray[numUsers-1].username, sizeof(userDataArray[numUsers].username), newData);

    gotoxy(28, tableStartY + numUsers * rowHeight - 2);
    enterData();
    strcpy_s(userDataArray[numUsers-1].password, sizeof(userDataArray[numUsers].password), newData);

    gotoxy(48, tableStartY + numUsers * rowHeight - 2);
    enterData();
    strcpy_s(userDataArray[numUsers-1].win, sizeof(userDataArray[numUsers].win), newData);

    gotoxy(59, tableStartY + numUsers * rowHeight - 2);
    enterData();
    strcpy_s(userDataArray[numUsers-1].lose, sizeof(userDataArray[numUsers].lose), newData);

    gotoxy(71, tableStartY + numUsers * rowHeight - 2);
    enterData();
    strcpy_s(userDataArray[numUsers-1].draw, sizeof(userDataArray[numUsers].draw), newData);

    strcpy(userDataArray[numUsers-1].role, "default");
    adminAddUser(sockfd, userDataArray[numUsers-1].username, userDataArray[numUsers-1].password, userDataArray[numUsers-1].role, atoi(userDataArray[numUsers-1].win), atoi(userDataArray[numUsers-1].lose), atoi(userDataArray[numUsers-1].draw));
    // update the UI
    frameUserManagement();
    displayUserData();
}

//ham sua thong tin nguoi dung, ko sua id và username
//cac buoc sua thong tin
//1. chon o can sua
//2. nhan [edit]
//3. nhap thong tin
//4. enter
void editUser() {
    int foundIndex;
    for (int i = 0; i < numUsers; i++) {
        if (userDataArray[i].id == userId) {
            foundIndex = i;
            break;
        }
    }
    if (selectedColumn == 4) {
        userDataArray[foundIndex].password[0] = '\0';
        frameUserManagement();
        displayUserData();
        gotoxy(28, tableStartY + foundIndex * rowHeight);
        enterData();
        strcpy_s(userDataArray[foundIndex].password, sizeof(userDataArray[foundIndex].password), newData);
    }
    else if (selectedColumn == 1) {
        userDataArray[foundIndex].win[0] = '\0';
        frameUserManagement();
        displayUserData();
        gotoxy(48, tableStartY + foundIndex * rowHeight);
        enterData();
        strcpy_s(userDataArray[foundIndex].win, sizeof(userDataArray[foundIndex].win), newData);
        printf("%d", selectedColumn);
    }
    else if (selectedColumn == 2) {
        userDataArray[foundIndex].lose[0] = '\0';
        frameUserManagement();
        displayUserData();
        gotoxy(59, tableStartY + foundIndex * rowHeight);
        enterData();
        strcpy_s(userDataArray[foundIndex].lose, sizeof(userDataArray[foundIndex].lose), newData);
    }
    else if (selectedColumn == 3) {
        userDataArray[foundIndex].draw[0] = '\0';
        frameUserManagement();
        displayUserData();
        gotoxy(71, tableStartY + foundIndex * rowHeight);
        enterData();
        strcpy_s(userDataArray[foundIndex].draw, sizeof(userDataArray[foundIndex].draw), newData);
    }
    else {
        printf("Invalid column selected.\n");
    }
    adminEditUser(sockfd, userDataArray[foundIndex].username, userDataArray[foundIndex].password, userDataArray[foundIndex].role, atoi(userDataArray[foundIndex].win), atoi(userDataArray[foundIndex].lose), atoi(userDataArray[foundIndex].draw));
    frameUserManagement();
    displayUserData();
}

//ham nhap du lieu 
void enterData() {
    int i = 0;
    char ch;
    while ((ch = _getch()) != '\r') { // Enter key
        if (ch == '\b') { // Backspace key
            if (i > 0) {
                i--;
                printf("\b \b"); // Move back, overwrite with space, move back again
            }
        }
        else if (ch != 0 && ch != '\xE0') { // Ignore special keys
            newData[i] = ch;
            i++;
            printf("%c", ch); // Echo the character
        }
    }
    newData[i] = '\0'; // Null-terminate the string
}

void addUserData(char *username, char *password, char *role, unsigned int wins, unsigned int losses, unsigned int draws) {
    if (username == NULL || password == NULL || role == NULL) return;
    strcpy(userDataArray[numUsers].username, username);
    strcpy(userDataArray[numUsers].password, password);
    strcpy(userDataArray[numUsers].role, role);
    sprintf(userDataArray[numUsers].win, "%d", wins);
    sprintf(userDataArray[numUsers].lose, "%d", losses);
    sprintf(userDataArray[numUsers].draw, "%d", draws);
    numUsers++;
}