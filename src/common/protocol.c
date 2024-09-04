#include "protocol.h"

#define BUFF_SIZE 255

Request *createRequest() {
    Request *ret = (Request *)malloc(sizeof(Request));
    if (ret == NULL)
        printf("MEMORY LEAKED!\n");
    else strcpy(ret->message, "\0");
    return ret;
}

Response *createResponse() {
    Response *ret = (Response *)malloc(sizeof(Response));
    if (ret == NULL)
        printf("MEMORY LEAKED!\n");
    else {
        strcpy(ret->message, "\0");
        strcpy(ret->data, "\0");
    }
    return ret;
}

int recvReq(int socket, Request *buff, int size, int flags) {
    int n = recv(socket, buff, size, flags);
    if (n < 0) printf("Failed to receive request!\n");
    return n;
}

int sendReq(int socket, Request *buff, int size, int flags) {
    int n = send(socket, buff, size, flags);
    if (n < 0) printf("Failed to send request!\n");
    return n;
}

int sendRes(int socket, Response *msg, int size, int flags) {
    int n = send(socket, msg, size, flags);
    if (n < 0) printf("Failed to send response!\n");
    return n;
}

int recvRes(int socket, Response *msg, int size, int flags) {
    int n = recv(socket, msg, size, flags);
    if (n < 0) printf("Failed to send receive!\n");
    return n;
}

void setMessageResponse(Response *msg) {
    if (msg->code != -1) {
        switch (msg->code) {
        case SYNTAX_ERROR:
            strcpy(msg->message, "Syntax error");
            break;
        case SIGN_UP_INPUT_WRONG:
            strcpy(msg->message, "Sign-up's input are wrong!");
            break;
        case SIGN_IN_INPUT_WRONG:
            strcpy(msg->message, "Sign-in's input are wrong!");
            break;
        case USERNAME_NOT_EXISTED:
            strcpy(msg->message, "This account is not registered!");
            break;
        case WRONG_PASSWORD:
            strcpy(msg->message, "Wrong password!");
            break;
        case USERNAME_EXISTED:
            strcpy(msg->message, "Username existed! Please choose another!");
            break;
        case ACCOUNT_BUSY:
            strcpy(msg->message, "This account is using by other player!");
            break;
        case SIGN_OUT_FAIL:
            strcpy(msg->message, "Sign-out got error!");
            break;
        case SIGN_IN_SUCCESS:
            strcpy(msg->message, "Sign in successfully!");
            break;
        case SIGN_UP_SUCCESS:
            strcpy(msg->message, "Sign up successfully!");
            break;
        case SIGN_OUT_SUCCESS:
            strcpy(msg->message, "Sign out successfully!");
            break;
        case WAITING_PLAYER:
            strcpy(msg->message, "No game matched the requirement!");
            break;
        case GAME_START:
            strcpy(msg->message, "Game started! Each turn has a duration of 30 seconds..");
            break;
        case YOUR_TURN:
            strcpy(msg->message, "Your turn!");
            break;
        case OTHER_PLAYER_TURN:
            strcpy(msg->message, "Opposite's turn!");
            break;
        case PICK_FAIL:
            strcpy(msg->message, "Pick failed!");
            break;
        case PICK_SUCCESS:
            strcpy(msg->message, "Pick successed!");
            break;
        case REDO_FAIL:
            strcpy(msg->message, "Sending redo's message failed!");
            break;
        case REDO_SUCCESS:
            strcpy(msg->message, "Redo is accepted!");
            break;
        case REDO_ASK_SUCCESS:
            strcpy(msg->message, "Redo is requested!");
            break;
        case YOU_WIN:
            strcpy(msg->message, "You won!");
            break;
        case OTHER_PLAYER_WIN:
            strcpy(msg->message, "Opposite won!");
            break;
        case QUIT_SUCCESS:
            strcpy(msg->message, "You have been quited!");
            break;
        case GET_USERS_SUCCESS:
            strcpy(msg->message, "Get users done!");
            break;
        case GET_USERS_CONTINUE:
            strcpy(msg->message, "Get users not done!");
            break;
        case GET_USERS_FAIL:
            strcpy(msg->message, "Get users failed!");
            break;
        case GET_GAMES_SUCCESS:
            strcpy(msg->message, "Get games done!");
            break;
        case GET_GAMES_FAIL:
            strcpy(msg->message, "Get games failed!");
            break;
        case ADD_USER_SUCCESS:
            strcpy(msg->message, "Added user!");
            break;
        case ADD_USER_FAIL:
            strcpy(msg->message, "User not added!");
            break;
        case EDIT_USER_SUCCESS:
            strcpy(msg->message, "Edited user!");
            break;
        case EDIT_USER_FAIL:
            strcpy(msg->message, "User not edited!");
            break;
        case DELETE_USER_SUCCESS:
            strcpy(msg->message, "Deleted user!");
            break;
        case DELETE_USER_FAIL:
            strcpy(msg->message, "User not deleted!");
            break;
        case DELETE_GAME_SUCCESS:
            strcpy(msg->message, "Deleted game!");
            break;
        case DELETE_GAME_FAIL:
            strcpy(msg->message, "Game not deleted!");
            break;
        case GET_REPLAY_SUCCESS:
            strcpy(msg->message, "Get replays done!");
            break;
        default:
            strcpy(msg->message, "No Message!");
            break;
        }
    }
}

void setOpcodeRequest(Request *req, char *input) {
  char code[BUFF_SIZE], data[BUFF_SIZE];

  splitMessage(input, code, data);
  strcpy(req->message, data);

    if (strcmp(code, STRING_CLOSE) == 0)
        req->code = CLOSE;
    else if (strcmp(code, STRING_SIGN_UP) == 0)
        req->code = SIGN_UP;
    else if (strcmp(code, STRING_SIGN_IN) == 0)
        req->code = SIGN_IN;
    else if (strcmp(code, STRING_SIGN_OUT) == 0)
        req->code = SIGN_OUT;
    else if (strcmp(code, STRING_FIND_GAME) == 0)
        req->code = FIND_GAME;
    else if (strcmp(code, STRING_PICK) == 0)
        req->code = PICK;
    else if (strcmp(code, STRING_REDO_ASK) == 0)
        req->code = REDO_ASK;
    else if (strcmp(code, STRING_REDO_AGREE) == 0)
        req->code = REDO_AGREE;
    else if (strcmp(code, STRING_QUIT) == 0)
        req->code = QUIT;
    else if (strcmp(code, STRING_GET_REPLAYS) == 0)
        req->code = GET_REPLAYS;
    else if (strcmp(code, STRING_GET_USERS) == 0)
        req->code = GET_USERS;
    else if (strcmp(code, STRING_GET_GAMES) == 0)
        req->code = GET_GAMES;
    else if (strcmp(code, STRING_ADD_USER) == 0)
        req->code = ADD_USER;
    else if (strcmp(code, STRING_ADD_USER) == 0)
        req->code = ADD_USER;
    else if (strcmp(code, STRING_EDIT_USER) == 0)
        req->code = EDIT_USER;
    else if (strcmp(code, STRING_DELETE_USER) == 0)
        req->code = DELETE_USER;
    else if (strcmp(code, STRING_DELETE_GAME) == 0)
        req->code = DELETE_GAME;
    else{
        req->code = -1;
    }
}

void splitMessage(char *input, char *code, char *data) {
	int i, codeLength = 0, dataLength = 0;

	for (i = 0; input[i] != ' '; i++) {
		code[codeLength++] = input[i];
	}
	code[codeLength] = '\0';
	i++;
	for (; i < strlen(input); i++)
	{
		data[dataLength++] = input[i];
	}
	data[dataLength] = '\0';
}