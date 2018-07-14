#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "fcntl.h"
#include "ctype.h"
#include "string.h"
#include "sys/file.h"
#include "sys/types.h"
#include "sys/stat.h"

int main(){
	int gold, fd;
	char goldbuf[3];
	FILE *mine;
	mine = fopen("/home/arthur/Eltex/IPC/Ps/mine.txt", "r+");
	fscanf(mine, "%d", &gold);
	fclose(mine);
	while(gold >= 0){
		if (gold == 0) {
			printf("Раб под номером: %d отправился в шахту, Рудник обрушился\n", getpid());
			exit(0);
		} else {
			mine = fopen("/home/arthur/Eltex/IPC/Ps/mine.txt", "w+");
			fscanf(mine, "%d", &gold);
			gold = gold - 5;
			sprintf(goldbuf, "%d", gold);
			printf("Раб под номером: %d отправился в шахту, Золото в руднике после работы = %d\n", getpid(), gold);
			fputs(goldbuf, mine);
			fflush(mine);
			fclose(mine);
		}
		sleep(3); // num of workers
	}
	printf("Закончил работу %d", getpid());
	return(gold);
}