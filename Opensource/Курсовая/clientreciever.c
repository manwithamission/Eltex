#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <string.h>

#define MAX_MSG_SIZE	  14
#define MAXRECVSTRING     255   /* Longest string to receive */
#define RCVBUFSIZE        100   /* Size of receive buffer */
#define BROADCASTPORTCLIENTRECEIVER 2002
#define TCPPORTCLIENTRECEIVER   2501  /* Port to TCP connections for managers*/


void Error(char *errorMessage) {
	perror(errorMessage);
	exit(1);
}

int main(int argc, char *argv[]) {	
	
	int sock;                         /* Socket */
	struct sockaddr_in broadcastAddr; /* Broadcast Address */
	unsigned short broadcastPort;     /* Broadcast port */
	char *recvString; /* Buffer for received string */
	int recvStringLen;                /* Length of received string */	

	int i = 0;
	uint8_t buf[MAX_MSG_SIZE]; // Input data container for bytes

	struct sockaddr_in src_addr;	  /* Echo server address */
	unsigned short serverport;     	  /* Echo server port */
	char *serveraddress;                     /* Server IP address (dotted quad) */
	// int bytesRcvd;					  /* Bytes read in single recv() and total bytes read */

	recvString = (char *) malloc(sizeof(char) * MAXRECVSTRING);
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		Error("socket() failed");
	}

	broadcastPort = htons(BROADCASTPORTCLIENTRECEIVER);

	memset(&broadcastAddr, 0, sizeof(broadcastAddr));   /* Zero out structure */
	broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
	broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);  /* Any incoming interface */
	broadcastAddr.sin_port = broadcastPort;             /* Broadcast port */

	if (bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0) {
		Error("bind() failed");
	}

	socklen_t src_addr_len = sizeof(src_addr);
	memset(&src_addr, 0x00, src_addr_len);

	// printf("%d:%d\n", broadcastAddr.sin_addr.s_addr, broadcastAddr.sin_port);

	while (1) {
		
		if ((recvStringLen = recvfrom(sock, recvString, MAXRECVSTRING, 0, (struct sockaddr *) &src_addr, &src_addr_len)) < 0) {
			// printf("че за хуйня\n");
			Error("recvfrom() failed");
		}
		// printf("че за хуйня\n");

		recvString[recvStringLen] = '\0';
		printf("UDP Broadcast received: %s\n", recvString);      		 /* Print the received string */
		printf("Server address: %s\n", inet_ntoa(src_addr.sin_addr));	 /* Print source ip */
		if ( strcmp(recvString, "Есть сообщения") == 0 ) {
			break;
		}
	}
	
	close(sock);

	serveraddress = inet_ntoa(src_addr.sin_addr);    /* server IP address (dotted quad) */
	serverport = TCPPORTCLIENTRECEIVER;			      /* Use given port */

	printf("ЧТО ТУТ У НАС%s %d\n", serveraddress, serverport);
	/* Create a reliable, stream socket using TCP */
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		Error("socket() failed");
	}

	/* Construct the server address structure */
	src_addr.sin_family      = AF_INET;             /* Internet address family */
	src_addr.sin_addr.s_addr = inet_addr(serveraddress);   /* Server IP address */
	src_addr.sin_port        = htons(serverport); 	/* Server port */

	if (connect(sock, (struct sockaddr *) &src_addr, sizeof(src_addr)) < 0) {
		Error("connect() failed");
	}

	while (1) {
		printf("Жду сообщений от сервера\n");
		

		while (1) {
			sleep(1);
			if (recv(sock, buf, 14, 0) == -1) {
				Error("recv() failed");
			}
			// if ((bytesRcvd = recv(sock, buf, 10, 0)) == -1) {
			// 	Error("recv() failed");
			// }

			printf("[%d]\tTCP recieved:\t%s\n", i, buf);
			i++;
		}

		break;
	}
	close(sock);
}