#include "top_screen.h"
#include "games.h"


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

void openAdminScreen() {
	if (MousePos.Y == 6 && MousePos.X >= 10 && MousePos.X <= 33) {
		// chuyen sang user management
		// frameUserManagement();
		getUsersData(sockfd);
	}
	else if (MousePos.Y == 10 && MousePos.X >= 10 && MousePos.X <= 33) {
		//chuyen sang man hinh replay
		// Cấp phát bộ nhớ động cho mảng replayDataArray
		replayDataArray = (ReplayData *)malloc(MAX_REPLAYS * sizeof(ReplayData));
		if (replayDataArray == NULL) {
			printf("Không thể cấp phát bộ nhớ\n");
			return;
		}
		// Data fetching from server
		fetchReplayInfoData();

		// Draw the initial UI
		drawReplayManagementUI();

		// Display the fetched data
		displayReplayData();
	}
	else if (MousePos.Y == 14 && MousePos.X >= 10 && MousePos.X <= 33) {
		dashboard();
	}
}
