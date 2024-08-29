#include <stdio.h>
#include <windows.h>

 void frameDashbroadAdmin() {
	system("cls");
	gotoxy(31, 1);
	printf("Admin");
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
	gotoxy(13, 6); printf("User Management");
	gotoxy(12, 10); printf("Replay Management");
	gotoxy(17, 14); printf("Sign out");
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
		 userManagement(); 
	 }else if (MousePos.Y == 14 && MousePos.X >= 10 && MousePos.X <= 33) {
		 drawInitialUI();
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
		 frameDashbroadAdmin();
		 openUser();
	 } else {
		 printf("ERROR");
	 }
 }


 

