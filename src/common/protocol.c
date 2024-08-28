#include "protocol.h"

#define BUFF_SIZE 255

Request *createRequest() {
    Request *ret = (Request *)malloc(sizeof(Request));
    if (ret == NULL)
        printf("MEMORY LEAKED!\n");
    return ret;
}

Response *createResponse() {
    Response *ret = (Response *)malloc(sizeof(Response));
    if (ret == NULL)
        printf("MEMORY LEAKED!\n");
    return ret;
}

int recvReq(int socket, Request *buff, int size, int flags) {
    int n = recv(socket, (char*)buff, size, flags);
    if (n < 0) printf("Failed to receive request!\n");
    return n;
}

int sendReq(int socket, Request *buff, int size, int flags) {
    int n = send(socket, (char*)buff, size, flags);
    if (n < 0) printf("Failed to send request!\n");
    return n;
}

int sendRes(int socket, Response *msg, int size, int flags) {
    int n = send(socket, (char*)msg, size, flags);
    if (n < 0) printf("Failed to send response!\n");
    return n;
}

int recvRes(int socket, Response *msg, int size, int flags) {
    int n = recv(socket, (char*)msg, size, flags);
    if (n < 0) printf("Failed to send receive!\n");
    return n;
}

void setMessageResponse(Response *msg) {
    if (msg->code != -1) {
        switch (msg->code) {
        case SYNTAX_ERROR:
            strcpy(msg->message, "Syntax error");
            break;
        case USERNAME_EXISTED:
            strcpy(msg->message, "Username existed! Please choose another!");
            break;
        case SIGN_UP_SUCCESS:
            strcpy(msg->message, "Sign up successfully!");
            break;
        case USERNAME_NOT_EXISTED:
            strcpy(msg->message, "This account is not registered!");
            break;
        case ACCOUNT_BUSY:
            strcpy(msg->message, "This account is using by other player!");
            break;
        case SIGN_IN_SUCCESS:
            strcpy(msg->message, "Sign in successfully!");
            break;
        case WRONG_PASSWORD:
            strcpy(msg->message, "Wrong password!");
            break;
        case SIGN_OUT_SUCCESS:
            strcpy(msg->message, "Sign out successfully!");
            break;
        case GAME_START:
            strcpy(msg->message, "Game started! Each turn has a duration of 30 seconds..");
            break;
        case YOUR_TURN:
            strcpy(msg->message, "Your turn!");
            break;
        case OTHER_PLAYER_TURN:
            strcat(msg->data, " 's turn!");
            strcpy(msg->message, msg->data);
            break;
        case PICK_FAIL:
            strcpy(msg->message, "Wrong number. Please pick another!");
            break;
        case PICK_SUCCESS:
            strcpy(msg->message, msg->data);
            strcat(msg->message, " is picked");
            break;
        case YOU_WIN:
            strcpy(msg->message, "You won!");
            break;
        case OTHER_PLAYER_WIN:
            strcat(msg->data, " won!");
            strcpy(msg->message, msg->data);
            break;
        case QUIT_SUCCESS:
            strcpy(msg->message, "You have been quited!");
            break;
        default:
            break;
        }
    }
}

void readMessageReponse(Response *msg) {
    if (msg->code != -1) {
        printf("%s\n", msg->message);
        switch (msg->code) {
        case SYNTAX_ERROR:
            printf("%s\n", msg->message);
            break;
        case SIGN_IN_SUCCESS:
            printf("Welcome %s\n", msg->data);
            break;
        case USERNAME_EXISTED:
            printf("%s\n", msg->message);
            break;
        default:
            break;
        }
    }
}

void setOpcodeRequest(Request *req, char *input) {
  char code[BUFF_SIZE], data[BUFF_SIZE];

  splitMessage(input, code, data);
  strcpy(req->message, data);

    if (strcmp(code, STRING_SIGN_UP) == 0)
        req->code = SIGN_UP;
    else if (strcmp(code, STRING_SIGN_IN) == 0)
        req->code = SIGN_IN;
    else if (strcmp(code, STRING_SIGN_OUT) == 0)
        req->code = SIGN_OUT;
    else if (strcmp(code, STRING_CLOSE) == 0)
        req->code = CLOSE;
    else if (strcmp(code, STRING_FIND_GAME) == 0)
        req->code = FIND_GAME;
    else if (strcmp(code, STRING_PICK) == 0)
        req->code = PICK;
    else if (strcmp(code, STRING_QUIT) == 0)
        req->code = QUIT;
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