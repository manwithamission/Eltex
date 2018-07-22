#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(){

	char message[1] = "1";
	// int gold = 0;
	int sock;
	struct sockaddr_in addr;
	const char *ip = "127.0.0.1";

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if(sock < 0){
		printf("socket error\n");
		exit(1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(1333); //порт
	addr.sin_addr.s_addr = inet_addr(ip);

	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		printf("connect error\n");
		exit(2);
	}

	while(1){
		send(sock, message, strlen(message), 0);
		sleep(1);
	}

	close(sock);
}