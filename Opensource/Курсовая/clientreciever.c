#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>

#define MAX_MSG_SIZE 14
#define MAXRECVSTRING 255
#define RCVBUFSIZE 100
#define BROADCASTPORTCLIENTRECEIVER 2002
#define TCPPORTCLIENTRECEIVER   2501


void Error(char *errorMessage) {
	perror(errorMessage);
	exit(1);
}

int main(int argc, char *argv[]) {	
	
	int sock;
	struct sockaddr_in broadcastAddr;
	unsigned short broadcastPort;
	char *recvString;
	int recvStringLen;

	int i = 1;
	uint8_t buf[MAX_MSG_SIZE];

	struct sockaddr_in src_addr;
	unsigned short serverport;
	char *serveraddress;

	recvString = (char *) malloc(sizeof(char) * MAXRECVSTRING);
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		Error("socket() failed");
	}

	broadcastPort = htons(BROADCASTPORTCLIENTRECEIVER);

	memset(&broadcastAddr, 0, sizeof(broadcastAddr));
	broadcastAddr.sin_family = AF_INET;
	broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	broadcastAddr.sin_port = broadcastPort;

	if (bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0) {
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
		if ( strcmp(recvString, "Есть сообщения") == 0 ) {
			break;
		}
	}
	
	close(sock);

	serveraddress = inet_ntoa(src_addr.sin_addr);
	serverport = TCPPORTCLIENTRECEIVER;

	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		Error("socket()");
	}

	src_addr.sin_family      = AF_INET;
	src_addr.sin_addr.s_addr = inet_addr(serveraddress);
	src_addr.sin_port        = htons(serverport);

	if (connect(sock, (struct sockaddr *) &src_addr, sizeof(src_addr)) < 0) {
		Error("connect()");
	}

	printf("Жду сообщений от сервера\n");
	while (1) {
		sleep(1);
		if (recv(sock, buf, 10, 0) == -1) {
			Error("recv()");
		}

		printf("[%d]Полученное TCP сообщение:%s\n", i, buf);
		i++;
		if (i == 6)	{
			break;
		}
	}
	close(sock);
}