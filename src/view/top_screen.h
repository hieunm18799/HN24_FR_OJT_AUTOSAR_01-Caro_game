#ifndef TOP_SCREEN_H
#define TOP_SCREEN_H

void drawTopNotLoginScreen();
void drawTopLoginedScreen();
void processLoginScreen(int sockfd);
void enterSignupCredentials(char *username, char *password, char *reenterPassword);
void enterCredentials(int sockfd, char *username, char *password);

#endif