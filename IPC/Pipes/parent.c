#include <sys/types.h> 
#include <sys/stat.h> 
#include <wait.h> 
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

struct truba {
 	int fd[2];
} tr[5];

int main(int argc, char *argv[]){
	int i, pid[6], gold;
	char buf[24];
	int gl[6] = {0,0,0,0,0,0};
	printf("Стартовое количество золота в руднике:");
	scanf("%d", &gold);
	for(i = 0; i < 6; i++) {
		pipe(tr[i].fd);
		pid[i]=fork();
		if(pid[i] == 0) {
			char str[6];
			sprintf(str, "%d\n", tr[i].fd[1]);
			execl("/home/arthur/Eltex/IPC/Pipes/child", "", str, NULL);
		}
	}
	while(gold > 0) {
		for(i = 0; i < 6; i++) {
			read(tr[i].fd[0], buf, 2);
			close(tr[i].fd[0]);
			gold = gold - atoi(buf);
			gl[i] += atoi(buf);
			printf("Раб под номером: %d несет %s золота из рудника, остаток %d\n", pid[i], buf, gold);
			if(gold == 0){
				printf("Рудник обрушился\n");
				for (i = 0; i < 6; i++){
					kill(pid[i], SIGKILL);
					printf("Раб %d убит\n", pid[i]);
					printf("Раб под номером: %d принес %d золота до обрушения\n", pid[i], gl[i]); 	
				}
			}
		}
	}	
return 0;
}