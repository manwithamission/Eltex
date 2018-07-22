#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

int main() {
	
	int server, listener, gold;
	char buf[1];
	struct sockaddr_in addr;
	int bytes_read;
	const char *ip = "127.0.0.1";
	printf("Установите кол-во золота в руднике: ");						//установка стартового кол-ва золота
	scanf("%d", &gold);

	listener = socket(AF_INET, SOCK_STREAM, 0);
	if(listener < 0){
		printf("socket\n");
		exit(1);
	}
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1333); //порт
	addr.sin_addr.s_addr = inet_addr(ip);

	if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		printf("bind\n");
		exit(2);
	}

	listen(listener, 1);
	
	while(1){

		server = accept(listener, NULL, NULL);

		if(server < 0){
			printf("accept\n");
			exit(3);
		}

		while(gold > 0) {
 
			bytes_read = recv(server, buf, 4, 0);	

			if(bytes_read <= 0){
				break;
			}

			gold = gold - atoi(buf);
			printf("Остаток золота в руднике: %d\n", gold);
		}	
		printf("Рудник обрушился\n");
		close(server);
	}	
	return 0;
}