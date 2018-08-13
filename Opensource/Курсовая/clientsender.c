#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <string.h>

#define MAXRECVSTRING     255
#define BROADCASTPORTCLIENTSENDER 2001
#define TCPPORTCLIENTSENDER 2500
#define STRNGLEN 69 

void Error(char *errorMessage) {
	perror(errorMessage);
	exit(1);
}

char *randstring(int length) {    
	char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
	size_t stringLen = STRNGLEN;        
	char *randomString;
	
	randomString = malloc(sizeof(char) * (length + 1));

	if (!randomString) {
		return (char*)0;
	}

	unsigned int key = 0;

	for (int n = 0; n < length; n++) {
		key = rand() % stringLen;
		randomString[n] = string[key];
	}

	randomString[length] = '\0';

	return randomString;
}

int main(int argc, char *argv[]) {	

	int sock;
	struct sockaddr_in broadcastAddr;
	unsigned short broadcastPort;
	char *recvString;
	int recvStringLen;	

	struct sockaddr_in src_addr;
	unsigned short serverport;
	char *serverIP;

	recvString = (char *) malloc(sizeof(char) * MAXRECVSTRING);
	if ( (sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
		Error("socket()");
	}

	broadcastPort = htons(BROADCASTPORTCLIENTSENDER);

	memset(&broadcastAddr, 0, sizeof(broadcastAddr));
	broadcastAddr.sin_family = AF_INET;
	broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	broadcastAddr.sin_port = broadcastPort;

	if ( bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0 ) {
		Error("bind()");
	}

	socklen_t src_addr_len = sizeof(src_addr);
	memset(&src_addr, 0x00, src_addr_len);

	while (1) {
	
		if ((recvStringLen = recvfrom(sock, recvString, MAXRECVSTRING, 0, (struct sockaddr *) &src_addr, &src_addr_len)) < 0) {
			Error("recvfrom()");
		}

		recvString[recvStringLen] = '\0';
		printf("UDP сообщение: %s\n", recvString);
		printf("Адресс сервера: %s\n", inet_ntoa(src_addr.sin_addr));
		if ( strcmp(recvString, "Жду сообщений") == 0 ) {
			break;
		}
	}
	
	close(sock);

	serverIP = inet_ntoa(src_addr.sin_addr);
	serverport = TCPPORTCLIENTSENDER;

	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		Error("socket()");
	}

	src_addr.sin_family      = AF_INET;
	src_addr.sin_addr.s_addr = inet_addr(serverIP);
	src_addr.sin_port        = htons(serverport);

	if (connect(sock, (struct sockaddr *) &src_addr, sizeof(src_addr)) < 0) {
		Error("connect()");
	}
	
	printf("Запускаю отправку сообщений серверу\n");

	for (int i = 1; i < 6; i++) {
		char *bufrndstr;
		bufrndstr = randstring(10);
		if (send(sock, bufrndstr, 10, 0) == -1) {
			Error("send()");	                 
		}
		printf("[%d]TCP сообщение отправлено:%s\n", i, bufrndstr);
		sleep(3);
	}
	close(sock);
}