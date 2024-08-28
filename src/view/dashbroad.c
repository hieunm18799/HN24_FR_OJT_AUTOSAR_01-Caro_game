#include <stdio.h>
#include <windows.h>
#include "vFunction.h"

int currentScreen = 4;

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

 int main() {
	 struct User* users = NULL;
	 initializeUsers(2);
	
	 dashbroad("admin", "addf");

		 while (1) {
			 handleMouseClick(); // Gọi hàm để xử lý sự kiện chuột

			 if (Click_flag) { // Nếu có sự kiện click
				 Click_flag = 0; // Reset cờ click

				 if (currentScreen == 5){// Nếu đang ở màn hình ban đầu
					 openAdmin();
				 } 
				 if (currentScreen == 7) {// Nếu đang ở màn hình ban đầu
					 addEditDelete(users, count);
				 }
				 if (currentScreen == 8) {// Nếu đang ở màn hình ban đầu
					 userInput();
				 }
				 if (currentScreen == 0) {
					 handleClickOnInitialScreen();
				 }
			 }
		 }

      return 0;
 }
 

