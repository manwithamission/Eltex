#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <string.h>

// #define MAX_MSG_SIZE	  14
#define MAXRECVSTRING     255
#define BROADCASTPORTCLIENTSENDER 2001
// #define BROADCASTPORTCLIENTRECEIVER 2002
#define TCPPORTCLIENTSENDER 2500
#define STRNGLEN 69 
// #define TCPPORTCLIENTRECEIVER 2501
// #define MSGKEY 10

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

	int sock;                         /* Socket */
	struct sockaddr_in broadcastAddr; /* Broadcast Address */
	unsigned short broadcastPort;     /* Broadcast port */
	char *recvString; /* Buffer for received string */
	int recvStringLen;                /* Length of received string */	

	struct sockaddr_in src_addr;	  /* Echo server address */
	unsigned short serverport;     	  /* Echo server port */
	char *serverIP;                      //Server IP address (dotted quad) 

	recvString = (char *) malloc(sizeof(char) * MAXRECVSTRING);
	if ( (sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
		Error("socket() failed");
	}

	broadcastPort = htons(BROADCASTPORTCLIENTSENDER);

	memset(&broadcastAddr, 0, sizeof(broadcastAddr));   /* Zero out structure */
	broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
	broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);  /* Any incoming interface */
	broadcastAddr.sin_port = broadcastPort;             /* Broadcast port */

	if ( bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0 ) {
		Error("bind() failed");
	}

	socklen_t src_addr_len = sizeof(src_addr);
	memset(&src_addr, 0x00, src_addr_len);

	// char tmpip[50] = "";
	// sprintf(tmpip, "%d", broadcastAddr.sin_addr.s_addr);
	// char tmpport[50] = "";
	// sprintf(tmpport, "%d", broadcastAddr.sin_port);
	// printf("%s:%s\n", tmpip, tmpport);

	while (1) {
	
		if ((recvStringLen = recvfrom(sock, recvString, MAXRECVSTRING, 0, (struct sockaddr *) &src_addr, &src_addr_len)) < 0) {
			Error("recvfrom() failed");
		}

		recvString[recvStringLen] = '\0';
		printf("UDP Broadcast received: %s\n", recvString);      		 /* Print the received string */
		printf("Server address: %s\n", inet_ntoa(src_addr.sin_addr));	 /* Print source ip */
		if ( strcmp(recvString, "Жду сообщений") == 0 ) {
			break;
		}
	}
	
	close(sock);

	serverIP = inet_ntoa(src_addr.sin_addr);		/* server IP address (dotted quad) */
	serverport = TCPPORTCLIENTSENDER;						/* Use given port */

	/* Create a reliable, stream socket using TCP */
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		Error("socket() failed");
	}
	printf("ЧТО ТУТ У НАС%s %d\n", serverIP, serverport);

	/* Construct the server address structure */
	src_addr.sin_family      = AF_INET;				/* Internet address family */
	src_addr.sin_addr.s_addr = inet_addr(serverIP);	/* Server IP address */
	src_addr.sin_port        = htons(serverport); 	/* Server port */

	if (connect(sock, (struct sockaddr *) &src_addr, sizeof(src_addr)) < 0) {
		Error("connect() failed");
	}
	
	printf("Start TCP sending\n");

	for (int i = 0; i < 5; i++) {
		char *bufrndstr;
		bufrndstr = randstring(10);
		if (send(sock, bufrndstr, 10, 0) == -1) {
			Error("send() sent a different number of bytes than expected");	                 
		}
		printf("[%d]\tTCP sended:\t%s\n", i, bufrndstr);
		sleep(3);
	}
	close(sock);
}