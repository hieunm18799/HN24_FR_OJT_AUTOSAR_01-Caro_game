#include <stdio.h>
#include "games.h"
#include <windows.h>
#include "top_screen.h"

#define MAX_USERS 9
int userId = 0;
int clickedRow;
char newData[50];

// Define structure to hold replay data
typedef struct {
    int id;
    char username[50];
    char password[50];
    char win[50];
    char lose[50];
    char draw[50];
} userData;

// Example array and number of users
userData userDataArray[MAX_USERS]; 
int numUsers = 5; // Number of users

void frameUserManagement();
void fetchUserData();
void displayUserData();
int handleUserRowClick();
void handleOnScreenUserManagement();
void deleteUser(int userId);
void addUser();
void sendDataToServer();
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
    printf("|   id   |   username   |     password     |   win   |   lose    |    draw    |");
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

// Function to fetch replay data from the server
void fetchUserData() {
    // Simulate fetching replay data (example data)
    for (int i = 0; i < MAX_USERS; i++) {
        sprintf_s(userDataArray[i].username, sizeof(userDataArray[i].username), "Username %d", i + 1);
        sprintf_s(userDataArray[i].password, sizeof(userDataArray[i].password), "Password %d", MAX_USERS - i);
        sprintf_s(userDataArray[i].win, sizeof(userDataArray[i].win), "%d", i);
        sprintf_s(userDataArray[i].lose, sizeof(userDataArray[i].lose), "%d", i);
        sprintf_s(userDataArray[i].draw, sizeof(userDataArray[i].draw), "%d", i);
        userDataArray[i].id = i+1;
    }
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

    // Determine which row was clicked
    if (MousePos.Y >= tableStartY && MousePos.Y < tableStartY + (numUsers * rowHeight)) {
        clickedRow = (MousePos.Y - tableStartY) / rowHeight;

        if (clickedRow < numUsers) {
            userId = userDataArray[clickedRow].id;
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

void deleteUser(int userId) {
    int foundIndex = -1;

    // Step 1: Find the replay by its ID
    for (int i = 0; i < numUsers; i++) {
        if (userDataArray[i].id == userId) {
            foundIndex = i;
            break;
        }
    }

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

        // Step 6: Update the UI
        frameUserManagement();
        displayUserData();

        printf("User with ID %d deleted.\n", userId);

        // Step 7: Send updated data to server (if needed)
        sendDataToServer();
    }
}

void addUser() {
    int foundIndex = -1;
    //add new row
    numUsers = numUsers + 1;
    userDataArray[numUsers].id = numUsers;
    userDataArray[numUsers].username[0] = '\0';
    userDataArray[numUsers].password[0] = '\0';
    userDataArray[numUsers].win[0] = '\0';
    userDataArray[numUsers].lose[0] = '\0';
    userDataArray[numUsers].draw[0] = '\0';

    // Update the UI
    frameUserManagement();
    displayUserData();
    printf("Enter new data");

    //enter and save the data
    gotoxy(13, tableStartY + numUsers * rowHeight - 2);
    enterData();
    strcpy_s(userDataArray[numUsers].username, sizeof(userDataArray[numUsers].username), newData);

    gotoxy(28, tableStartY + numUsers * rowHeight - 2);
    enterData();
    strcpy_s(userDataArray[numUsers].password, sizeof(userDataArray[numUsers].password), newData);

    gotoxy(48, tableStartY + numUsers * rowHeight - 2);
    enterData();
    strcpy_s(userDataArray[numUsers].win, sizeof(userDataArray[numUsers].win), newData);

    gotoxy(59, tableStartY + numUsers * rowHeight - 2);
    enterData();
    strcpy_s(userDataArray[numUsers].lose, sizeof(userDataArray[numUsers].lose), newData);

    gotoxy(71, tableStartY + numUsers * rowHeight - 2);
    enterData();
    strcpy_s(userDataArray[numUsers].draw, sizeof(userDataArray[numUsers].draw), newData);

    // update the UI
    frameUserManagement();
    displayUserData();
}

// Function to send data back to the server after deletion
void sendDataToServer() {
    printf("Data sent back to the server.\n");
}

void editUser() {
    int foundIndex;
    for (int i = 0; i < numUsers; i++) {
        if (userDataArray[i].id == userId) {
            foundIndex = i;
            break;
        }
    }

    userDataArray[foundIndex].password[0] = '\0';
    userDataArray[foundIndex ].win[0] = '\0';
    userDataArray[foundIndex ].lose[0] = '\0';
    userDataArray[foundIndex ].draw[0] = '\0';

    // Update the UI
    frameUserManagement();
    displayUserData();
    printf("Enter data");

    gotoxy(28, tableStartY + foundIndex * rowHeight);
    enterData();
    strcpy_s(userDataArray[foundIndex].password, sizeof(userDataArray[foundIndex].password), newData);

    gotoxy(48, tableStartY + foundIndex * rowHeight);
    enterData();
    strcpy_s(userDataArray[foundIndex].win, sizeof(userDataArray[foundIndex].win), newData);

    gotoxy(59, tableStartY + foundIndex * rowHeight);
    enterData();
    strcpy_s(userDataArray[foundIndex ].lose, sizeof(userDataArray[foundIndex].lose), newData);

    gotoxy(71, tableStartY + foundIndex * rowHeight );
    enterData();
    strcpy_s(userDataArray[foundIndex].draw, sizeof(userDataArray[foundIndex].draw), newData);

    // update the UI
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


