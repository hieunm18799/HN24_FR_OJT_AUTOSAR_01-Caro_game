#include <stdio.h>
#include <windows.h>
#include "top_screen.h"

// int currentScreen = 4;

void frameDashbroadAdmin(char* username) {
	system("cls");
	gotoxy(27, 1);
	printf("%s", username);
	gotoxy(17, 3);
	printf("CARO GAME");
	for (int i = 2; i <= 7; i++) {
		if (i <= 5) {
			gotoxy(10, 4 * i - 2);
			printf("[");
			gotoxy(33, 4 * i - 2);
			printf("]");
		}
	}
	gotoxy(17, 6);  printf("Play game");
	gotoxy(17, 10); printf("Replay list");
	gotoxy(13, 14); printf("Admin management");
	gotoxy(17, 18); printf("Sign out");
	gotoxy(1, 20);
	currentScreen = VIEW_TOP_SIGNED_IN_ADMIN;
}

 void frameDashbroadUser(char *username) {
	 system("cls");
	 gotoxy(27, 1);
	 printf("%s", username);
	 gotoxy(17, 3);
	 printf("CARO GAME");
	 for (int i = 2; i <= 7; i++) {
		 if (i <= 4) {
			 gotoxy(10, 4 * i - 2);
			 printf("[");
			 gotoxy(33, 4 * i - 2);
			 printf("]");
		 }
	 }
	 gotoxy(17, 6);  printf("Play game");
	 gotoxy(17, 10); printf("Replay list");
	 gotoxy(17, 14); printf("Sign out");
	 gotoxy(1, 20);
	 currentScreen = VIEW_TOP_SIGNED_IN_USER;
 }
 
 //open new screen from adminScreen
 void openAdmin() {
	 if (MousePos.Y == 6 && MousePos.X >= 10 && MousePos.X <= 33) {
		 //man hinh playgame 
	 }else if (MousePos.Y == 18 && MousePos.X >= 10 && MousePos.X <= 33) {
		 drawInitialUI();
	 }
	 else if (MousePos.Y == 14 && MousePos.X >= 10 && MousePos.X <= 33) {
		//  adminManagement();
	 }else if (MousePos.Y == 10 && MousePos.X >= 10 && MousePos.X <= 33) {
		 //chuyen sang man hinh replay 
	 }
 }

 //open new screen from userCreen
 void openUser() {
	 if (MousePos.Y == 6 && MousePos.X >= 10 && MousePos.X <= 33) {
		 //chuyen sang man hình playgame
	 }
	 else if (MousePos.Y == 10 && MousePos.X >= 10 && MousePos.X <= 33) {
		 //chuyen sang man hinh replay
	 }
	 else if (MousePos.Y == 14 && MousePos.X >= 10 && MousePos.X <= 33) {
		 drawInitialUI();
	 }
 }

 void dashbroad() {
	 if (strcmp(signed_in_role, "admin") == 0) {
		 frameDashbroadAdmin(signed_in_username);
	 } else {
		 frameDashbroadUser(signed_in_username);
	 }
 }
