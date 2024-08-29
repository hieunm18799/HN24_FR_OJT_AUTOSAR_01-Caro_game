#include <stdio.h>
#include <windows.h>
#include "top_screen.h"


void frameAdminScreen() {
	system("cls");
	gotoxy(27, 1);
	gotoxy(15, 3);
	printf("ADMIN DASHBOARD");
	for (int i = 2; i <= 7; i++) {
		if (i <= 4) {
			gotoxy(10, 4 * i - 2);
			printf("[");
			gotoxy(33, 4 * i - 2);
			printf("]");
		}
	}
	gotoxy(13, 6);  printf("User Management");
	gotoxy(13, 10); printf("Replay Management");
	gotoxy(17, 14); printf("Back");
	currentScreen = VIEW_ADMIN_MANAGE;
}

void openAdminScreen(char* role, char* username) {
	if (MousePos.Y == 6 && MousePos.X >= 10 && MousePos.X <= 33) {
		// chuyen sang user management
	}
	else if (MousePos.Y == 10 && MousePos.X >= 10 && MousePos.X <= 33) {
		//chuyen sang man hinh replay
	}
	else if (MousePos.Y == 14 && MousePos.X >= 10 && MousePos.X <= 33) {
		dashboard(role, username);
	}
}
