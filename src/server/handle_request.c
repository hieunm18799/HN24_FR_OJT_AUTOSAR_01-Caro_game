#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "protocol.h"
#include "users.h"

bool signup(int clientfd, Request *req, Response* res);
bool signin(int clientfd, Request *req, Response *res);
bool signout(int clientfd, Request *req, Response *res);
bool signoutByX(int clientfd, Request *req, Response *res);
bool startGame(int sockfd, int clientfd, Request *req, Response *res);
bool play(int clientfd, Request *req, Response *res);
bool pick(int clientfd, Request *req, Response *res);
bool quit(int clientfd, Request *req, Response *res);