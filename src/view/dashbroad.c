#include <stdio.h>
#include <windows.h>
#include "vFunction.h"

int currentScreen = 4;

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
	currentScreen = 5;
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
	 currentScreen = 6;
 }
 
 //open new screen from adminScreen
 void openAdmin() {
	 if (MousePos.Y == 6 && MousePos.X >= 10 && MousePos.X <= 33) {
		 //man hinh playgame 
	 }else if (MousePos.Y == 18 && MousePos.X >= 10 && MousePos.X <= 33) {
		 drawInitialUI();
	 }
	 else if (MousePos.Y == 14 && MousePos.X >= 10 && MousePos.X <= 33) {
		 adminManagement();
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

 void dashbroad(char *role, char *username) {
	 if (strcmp(role, "user") == 0) {
		 frameDashbroadUser(username);
		 openAdmin();
	 } else if (strcmp(role, "admin") == 0) {
		 frameDashbroadAdmin(username);
		 openUser();
	 } else {
		 printf("ERROR");
	 }
 }
