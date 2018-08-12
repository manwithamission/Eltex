
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <time.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define BROADCASTPORTCLIENTSENDER 2001
#define BROADCASTPORTCLIENTRECEIVER 2002
#define TCPPORTCLIENTSENDER 2500
#define TCPPORTCLIENTRECEIVER 2501

struct mesg_buffer {
    long mesg_type;
    uint8_t mesg_text[20];
} message;

int msgid;
int length;
struct msqid_ds msqid_ds, *buf;
// buf = &msqid_ds;

char *broadcastaddress;

void Error(char *errorMessage) {
	perror(errorMessage);
	exit(1);
}

int queue() {

	key_t key;
	int msgid;
	// ftok to generate unique key
	key = ftok(".", 65);

	msgid = msgget(key, 0666 | IPC_CREAT);
	message.mesg_type = 1;

	return msgid;
}

int CreateTCPServerSocket(unsigned short port) {
	int serversocket;							/* socket to create */
	struct sockaddr_in addr;			/* Local address */

	/* Create socket for incoming connections */
	if ( (serversocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ) {
		Error("socket error");
	}
	
	/* Construct local address structure */
	memset(&addr, 0, sizeof(addr));				/* Zero out structure */
	addr.sin_family      = AF_INET;				/* Internet address family */
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	/* Server IP address */
	addr.sin_port        = htons(port); 	/* Server port */

	/* Bind to the local address */
	if ( bind(serversocket, (struct sockaddr *) &addr, sizeof(addr)) < 0 ) {
		Error("bind error");
	}

	/* Mark the socket so it will listen for incoming connections */
	if ( listen(serversocket, 5) < 0 ) {
		Error("listen error");
	}

	return serversocket;
}

void *UdpBroadcastSenderForClientSender(void *arg) {

	int sock;									/* Socket */
	struct sockaddr_in broadcastAddr;			/* Broadcast address */
	unsigned short broadcastPort;				/* Server port */
	const char *sendString = "Жду сообщений";
	int broadcastPermission;					/* Socket opt to set permission to broadcast */
	unsigned int sendStringLen;					/* Length of string to broadcast */

	// struct msqid_ds msqid_ds, *buf;				//msgctl
	buf = &msqid_ds;
	// int msgid = queue();
	msgctl(msgid, IPC_STAT, buf);

	broadcastPort = htons(BROADCASTPORTCLIENTSENDER );		/* Broadcast port */

	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		Error("socket error");
	}

	broadcastPermission = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, sizeof(broadcastPermission)) < 0) {
		Error("setsockopt error");
	}

	memset(&broadcastAddr, 0, sizeof(broadcastAddr));		/* Zero out structure */
	broadcastAddr.sin_family = AF_INET;						/* Internet address family */
	broadcastAddr.sin_addr.s_addr = inet_addr(broadcastaddress); /* Broadcast IP address */
	broadcastAddr.sin_port = broadcastPort;					/* Broadcast port */
	sendStringLen = strlen(sendString);						/* Find length of sendString */
	
	while (1) { /* Run forever */
		sleep(1);
		printf("Сообщений в очереди UDP для клиента-отправителя %ld\n", buf->msg_qnum);
		if (buf->msg_qnum < 1) {
			sleep(1);
			if (sendto(sock, sendString, sendStringLen, 0, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) != sendStringLen) {
				Error("sendto() sent a different number of bytes than expected 1");
			}
		}
	}
}

void *UdpBroadcastSenderForClientReceiver(void *arg) {

	int sock;									/* Socket */
	struct sockaddr_in broadcastAddr;			/* Broadcast address */
	unsigned short broadcastPort;				/* Server port */
	const char *sendString = "Есть сообщения";
	int broadcastPermission;					/* Socket opt to set permission to broadcast */
	unsigned int sendStringLen;					/* Length of string to broadcast */

	buf = &msqid_ds;
	msgctl(msgid, IPC_STAT, buf);

	broadcastPort = htons(BROADCASTPORTCLIENTRECEIVER );		/* Broadcast port */

	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		Error("socket error");
	}

	broadcastPermission = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, sizeof(broadcastPermission)) < 0) {
		Error("setsockopt error");
	}

	memset(&broadcastAddr, 0, sizeof(broadcastAddr));		/* Zero out structure */
	broadcastAddr.sin_family = AF_INET;						/* Internet address family */
	broadcastAddr.sin_addr.s_addr = inet_addr(broadcastaddress); /* Broadcast IP address */
	broadcastAddr.sin_port = broadcastPort;					/* Broadcast port */
	sendStringLen = strlen(sendString);						/* Find length of sendString */
	
	while (1) { /* Run forever */
		sleep(1);
		printf("Сообщений в очереди UDP для клиента-получателя %ld\n", buf->msg_qnum);
		if (buf->msg_qnum > 0) {
			sleep(1);
			if (sendto(sock, sendString, sendStringLen, 0, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) != sendStringLen) {
				Error("sendto() sent a different number of bytes than expected 2");
			}
		}
	}
}

void *TcpConnectionClientSender(void *arg) {
	int serversocket;					/* Socket descriptor for server */
	int clientsocket;					/* Socket descriptor for client */
	unsigned short serverport;			/* Server port */
	struct sockaddr_in echoClntAddr;	/* Client address */
	unsigned int clntLen;
	clntLen = sizeof(echoClntAddr);
	int recvMsgSize;
	
	// struct msqid_ds msqid_ds, *buf;
	buf = &msqid_ds;
	// int msgid = queue();
	
	serverport = TCPPORTCLIENTSENDER;   /* Local port */

	serversocket = CreateTCPServerSocket(serverport);

	while (1) {
		if ((clientsocket = accept(serversocket, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0) {
			Error("accept() failed");
		}
		printf("Жду сообщений от клиента-отправителя\n");

		while (1) {
			if ((recvMsgSize = recv(clientsocket, message.mesg_text, 10, 0)) < 0) {
				Error("recv() failed");
			}
			if (recvMsgSize > 0) {
				message.mesg_type = 1;
				// *message.mesg_text = *buf;
				printf("Буфер очереди %s\n", message.mesg_text);
				msgsnd(msgid, &message, length, 1);
				msgctl(msgid, IPC_STAT, buf);
				printf("Сообщений в очереди после отправки %ld\n", buf->msg_qnum);
				// printf("Буфер %s\n", buf);
			} else {
				msgctl(msgid, IPC_STAT, buf);
				printf("%ld\n", buf->msg_qnum);
				// msgctl(msgid, IPC_RMID, 0);
				break;
			}
		}
	}
}

void *TcpConnectionClientReceiver(void *arg) {
	int serversocket;					/* Socket descriptor for server */
	int clientsocket;					/* Socket descriptor for client */
	unsigned short serverport;			/* Server port */
	struct sockaddr_in echoClntAddr;	/* Client address */
	unsigned int clntLen;
	clntLen = sizeof(echoClntAddr);
	int i = 0;
	
	// struct msqid_ds msqid_ds, *buf;
	buf = &msqid_ds;
	// int msgid = queue();
	
	serverport = TCPPORTCLIENTRECEIVER;   /* Local port */

	serversocket = CreateTCPServerSocket(serverport);

	while (1) {
		printf("Жду подключения клиента-получателя\n");
		if ((clientsocket = accept(serversocket, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0) {
			Error("accept() failed");
		}
		while (1) {
			msgctl(msgid, IPC_STAT, buf);
			if (buf->msg_qnum > 0) {
				sleep(3);
				if (msgrcv(msgid, &message, length, message.mesg_type, 0) < 0) {
					Error("msgrecv()");
				}
				printf("%ld\n", buf->msg_qnum);
				if (send(clientsocket, message.mesg_text, 10, 0) < 0) {
					Error("send() sent a different number of bytes than expected");	                 
				}
				printf("[%d]\tОтправлено клиенту-получателю:\t%s\n", i, message.mesg_text);
				i++;
			}
		}
		msgctl(msgid, IPC_RMID, 0);
	}
}

int main(int argc, char *argv[]) {
	msgid = queue();
	length = sizeof(message) - sizeof(long);
	int threadcount = 4;
	pthread_t threads[threadcount];
	void *status[threadcount];
	
	broadcastaddress = (char *) malloc(sizeof(char)*15);
 
	if (argc < 2) {
		printf("Syntax: <BroadcastIP>\n");
		exit(1);	
	}

	printf("Broadcast IP: %s\n", argv[1]);
	broadcastaddress = argv[1];

	pthread_create(&threads[1], NULL, UdpBroadcastSenderForClientSender, 0);
	pthread_create(&threads[2], NULL, TcpConnectionClientSender, 0);
	pthread_create(&threads[3], NULL, UdpBroadcastSenderForClientReceiver, 0);
	pthread_create(&threads[4], NULL, TcpConnectionClientReceiver, 0);

	pthread_join(threads[1], &status[1]);
	pthread_join(threads[2], &status[2]);
	pthread_join(threads[3], &status[3]);
	pthread_join(threads[4], &status[4]);
}