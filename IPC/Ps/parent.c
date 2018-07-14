#include "sys/wait.h"
#include "signal.h"
#include "sys/stat.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include <fcntl.h> 
#include "sys/file.h"


void mining() {
	int i, worker, gold, status, fd;
	// char goldbuf[3];
	FILE *mine;
	printf("Введите количество золота в руднике: \n");
	scanf("%d", &gold);
	printf("Установленное количество золота в руднике: %d\n", gold);
	printf("Введите число рабочих: \n");
	scanf("%d", &worker);
	int pid[worker];
	mine = fopen("/home/arthur/Eltex/IPC/Ps/mine.txt", "r+");
	fprintf(mine, "%d", gold);
	fflush(mine);
	fclose(mine);
	
	for(i = 0; i < worker; i++) {
		sleep(1);
		pid[i] = fork();
		if(pid[i] == 0) {
			execl("/home/arthur/Eltex/IPC/Ps/child", "", NULL);
		} else if (pid[i] == -1){
			printf("Не удалось породить процесс");
			exit(1);
		} 
	}
}

int main(){
	mining();
	return 0;
}
