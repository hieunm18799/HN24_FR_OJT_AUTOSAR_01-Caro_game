#include <stdio.h>
#include <windows.h>
#include "top_screen.h"

void frameDashboardAdmin(char* role, char* username) {
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

	currentScreen = VIEW_TOP_SIGNED_IN_ADMIN;
}

 void frameDashboardUser() {
	 system("cls");
	 gotoxy(27, 1);
	 printf("%s", signed_in_username);
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
   
	 currentScreen = VIEW_TOP_SIGNED_IN_USER;

 }
 
 //open new screen from adminScreen
 void openAdmin() {
	 if (MousePos.Y == 6 && MousePos.X >= 10 && MousePos.X <= 33) {
		 //man hinh playgame
		 startGame(sockfd, signed_in_username);
	 }
	else if (MousePos.Y == 10 && MousePos.X >= 10 && MousePos.X <= 33) {
		 //chuyen sang man hinh replay 
	 }
	 else if (MousePos.Y == 14 && MousePos.X >= 10 && MousePos.X <= 33) {
		 frameAdminScreen();
	 }
	 else if (MousePos.Y == 18 && MousePos.X >= 10 && MousePos.X <= 33) {
		 signout(sockfd, signed_in_username);
	 }
 }

 //open new screen from userCreen
 void openUser() {
	 if (MousePos.Y == 6 && MousePos.X >= 10 && MousePos.X <= 33) {
		 //chuyen sang man hình playgame
		 startGame(sockfd, signed_in_username);
	 }
	 else if (MousePos.Y == 10 && MousePos.X >= 10 && MousePos.X <= 33) {
		 //chuyen sang man hinh replay
	 }

	 else if (MousePos.Y == 14 && MousePos.X >= 10 && MousePos.X <= 33) {
		 signout(sockfd, signed_in_username);
	 }
 }

 void dashboard() {
	 if (strcmp(signed_in_role, "admin") == 0) {
		 frameDashboardAdmin(signed_in_role, signed_in_username);
	 } else
		 frameDashboardUser( signed_in_role, signed_in_username);
 }
 
