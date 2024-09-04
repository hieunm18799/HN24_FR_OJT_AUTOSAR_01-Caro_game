#include "games.h"
#include "users.h"

#define WIN_CONDITION 4

static int checkMove(Move *moves, unsigned char x, unsigned char y, unsigned char flag);
static int checkWin(Game *game, unsigned char x, unsigned char y);

RES_OPCODE pickCaro(char* username, unsigned int game_id, unsigned char x, unsigned char y, SOCKET *oppfd) {
    Game *curGame = findGameById(game_id);

    if (curGame == NULL || curGame->status == END || checkMove(curGame->moves, x, y, 0)) return PICK_FAIL;
    if (strcmp(username, curGame->player1_name) == 0 && curGame->status == PLAYER2 || strcmp(username, curGame->player2_name) == 0 && curGame->status == PLAYER1) return OTHER_PLAYER_TURN;

    if (curGame->board_height < x + 1) curGame->board_height = x + 1;
    if (curGame->board_width < y + 1) curGame->board_width = y + 1;

    int res = addMove(curGame, x, y);

    if (res == 0) return PICK_FAIL;
    User *oppUser = findUserByName(strcmp(username, curGame->player1_name) == 0 ? curGame->player2_name : curGame->player1_name);
    *oppfd = oppUser->clientfd;
    res = checkWin(curGame, x, y);
    curGame->status = curGame->status == PLAYER1 ? PLAYER2 : PLAYER1;
    if (res) {
        changeGame(curGame, "\0", "\0", username, END);
        increasedWins(findUserByName(username));
        increasedLosses(findUserByName(oppUser->username));
        writeUsersIni();
        // Tạo đối tượng MatchHistory từ dữ liệu Game
        // Đảm bảo rằng matchHistoryList là con trỏ
        MatchHistory* global_replay = NULL; // Đây là con trỏ kiểu MatchHistory*

        // Thêm trận đấu vào danh sách
        addReplay(&global_replay, curGame->player1_name, curGame->player2_name, curGame->id, curGame->result, curGame->moves);

        // Gọi hàm lưu lịch sử trận đấu
        saveMatchHistoryToIniFile(global_replay, "Re_play.ini");

        // Giải phóng bộ nhớ đã cấp phát cho danh sách liên kết
        while (global_replay != NULL) {
            MatchHistory* temp = global_replay;
            global_replay = global_replay->next;
            free(temp);
        }
        return YOU_WIN;
    }
    return PICK_SUCCESS;
}

RES_OPCODE redoAsk(char* username, unsigned int game_id, SOCKET *oppfd) {
    Game *curGame = findGameById(game_id);

    if (curGame == NULL || curGame->status == END || strcmp(username, curGame->player1_name) == 0 && curGame->status == PLAYER1 || strcmp(username, curGame->player2_name) == 0 && curGame->status == PLAYER2) return REDO_FAIL;
    char oppUserName[MAX_LENGTH];
    strcpy(oppUserName, strcmp(curGame->player1_name, username) == 0 ? curGame->player2_name : curGame->player1_name);

    *oppfd = findUserByName(oppUserName)->clientfd;
    return REDO_ASK_SUCCESS;
}

RES_OPCODE redoAgree(char* username, unsigned int game_id, SOCKET *oppfd, unsigned char *x, unsigned char *y) {
    Game *curGame = findGameById(game_id);

    if (curGame == NULL || curGame->status == END || strcmp(username, curGame->player1_name) == 0 && curGame->status == PLAYER2 || strcmp(username, curGame->player2_name) == 0 && curGame->status == PLAYER1) return REDO_FAIL;
    
    char oppUserName[MAX_LENGTH];
    strcpy(oppUserName, strcmp(curGame->player1_name, username) == 0 ? curGame->player2_name : curGame->player1_name);
    *oppfd = findUserByName(oppUserName)->clientfd;

    redoMove(curGame, x, y);
    return REDO_SUCCESS;
}

RES_OPCODE quitLogic(char* username, unsigned int game_id, SOCKET *oppfd) {
    Game *curGame = findGameById(game_id);

    if (curGame == NULL) return QUIT_SUCCESS;
    switch (curGame->status) {
        case NOT_PLAY:
            deleteGame(game_id);
        case END:
            return QUIT_SUCCESS;
            break;
        default:
            break;
    }

    char oppUserName[MAX_LENGTH];
    strcpy(oppUserName, strcmp(curGame->player1_name, username) == 0 ? curGame->player2_name : curGame->player1_name);
    *oppfd = findUserByName(oppUserName)->clientfd;

    changeGame(curGame, "\0", "\0", oppUserName, END);
    increasedWins(findUserByName(oppUserName));
    increasedLosses(findUserByName(username));
    writeUsersIni();
    MatchHistory* global_replay = NULL; // Đây là con trỏ kiểu MatchHistory*

    // Thêm trận đấu vào danh sách
    addReplay(&global_replay, curGame->player1_name, curGame->player2_name, curGame->id, curGame->result, curGame->moves);

    // Gọi hàm lưu lịch sử trận đấu
    saveMatchHistoryToIniFile(global_replay, "Re_play.ini");

    // Giải phóng bộ nhớ đã cấp phát cho danh sách liên kết
    while (global_replay != NULL) {
        MatchHistory* temp = global_replay;
        global_replay = global_replay->next;
        free(temp);
    }
    return QUIT_SUCCESS;
}

static int checkMove(Move *moves, unsigned char x, unsigned char y, unsigned char flag) {
    Move *temp;
    if (flag != PLAYER2) temp = moves;
    else if (moves->next != NULL) temp = moves->next;
    while (temp != NULL) {
        if (temp->x == x && temp->y == y) return 1;
        if (flag == 0) temp = temp->next;
        else {
            if (temp->next != NULL) temp = temp->next->next;
            else temp = NULL;
        }
    }
    return 0;
}

static int checkWin(Game *game, unsigned char last_x, unsigned char last_y) {
    // Check horizontal
    int count = 1;
    for (int i = last_x - 1; i >= 0; i--) {
        if (!checkMove(game->moves, i, last_y, game->status)) break;
        count++;
    }
    for (int i = last_x + 1; i < game->board_width; i++) {
        if (!checkMove(game->moves, i, last_y, game->status)) break;
        count++;
    }
    if (count >= WIN_CONDITION) return 1;

    // Check vertical
    count = 1;
    for (int i = last_y - 1; i >= 0; i--) {
        if (!checkMove(game->moves, last_x, i, game->status)) break;
        count++;
    }
    for (int i = last_y + 1; i < game->board_height; i++) {
        if (!checkMove(game->moves, last_x, i, game->status)) break;
        count++;
    }
    if (count >= WIN_CONDITION) return 1;

    // Check diagonal from top-left to bottom-right
    count = 1;
    for (int i = 1; last_x - i >= 0 && last_y - i >= 0; i++) {
        if (!checkMove(game->moves, last_x - i, last_y - i, game->status)) break;
        count++;
    }
    for (int i = 1; last_x + i < game->board_width && last_y + i < game->board_height; i++) {
        if (!checkMove(game->moves, last_x + i, last_y + i, game->status)) break;
        count++;
    }
    if (count >= WIN_CONDITION) return 1;

    // Check diagonal from bottom-left to top-right
    count = 1;
    for (int i = 1; last_x - i >= 0 && last_y + i < game->board_height; i++) {
        if (!checkMove(game->moves, last_x - i, last_y + i, game->status)) break;
        count++;
    }
    for (int i = 1; last_x + i < game->board_width && last_y - i >= 0; i++) {
        if (!checkMove(game->moves, last_x + i, last_y - i, game->status)) break;
        count++;
    }
    if (count >= WIN_CONDITION) return 1;
    return 0;
}