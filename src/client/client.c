#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
// For ..
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "protocol.h"

#define BUFF_SIZE 255

void recv_handler(int serverfd);

int main(int argc, char const *argv[]) {
    // input: IPv4 + Port
	if(argc != 3) {
		printf("Please input IP address and port number\n");
		return 0;
	}

    struct sockaddr_in servaddr;
    // Create TCP socket
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        perror("CREATE_SOCKET_ERROR");
        exit(0);
    }
    //Step 2: Specify server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    socklen_t len = sizeof(struct sockaddr_in);

    int check = connect(sockfd, (struct sockaddr *)&servaddr, len);
    if (check == -1) {
        perror("CONNECT_ERROR");
        exit(0);
    }
    recv_handler(sockfd);

    close(sockfd);
    return 0;
}

void recv_handler(int serverfd) {
    int rcvBytes;
    Response *res = (Response *)malloc(sizeof(Response));
    
    while (1)
    {

        rcvBytes = recvRes(serverfd, res, sizeof(Response), 0);
        if (rcvBytes < 0) {
            perror("\nError: ");
            break;
        }
        if (strcmp("", res->data) != 0)
            printf("\nCode: %d\nMessage: %s\nData: %s\n", res->code, res->message, res->data);
        else
            printf("\nCode: %d\nMessage: %s\n", res->code, res->message);
    }

    close(serverfd);
    return;
}