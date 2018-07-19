#include <sys/types.h> 
#include <sys/stat.h> 
#include <wait.h> 
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct mymsgbuf {
 	long mtype;
 	int one;
 	int pid;
} mess_t;

mess_t buf;
int i, gold, worker;
int qid;
key_t msgkey;
int *gl;
int length;

int main(int argc, char *argv[]) {
	printf("Укажите кол-во рабочих:");
	scanf("%d", &worker);
	int pid[worker];

	gl = (int*)malloc(worker * sizeof(int)); 									//выделение памяти

	for (i = 0; i < worker; i++) {												//инициализация массива рабочих
		gl[i] = 0;
	}

	length = sizeof(mess_t) - sizeof(long);
	msgkey = ftok(".", 'm');
	qid = msgget(msgkey, IPC_CREAT | 0660);
	printf("Установите золото в руднике: ");
	scanf("%d", &gold);
	for(i = 0; i < worker; i++) {
		pid[i]=fork();
		if(pid[i] == 0) {
			// printf("SON - QID = %d\n", qid);
			while(1) {
				buf.mtype = 1;
				buf.one = 1;
				buf.pid = getpid();
				msgsnd(qid, &buf, length, 1);
				sleep(1);
			}
		}
	}
	// printf("FATHER - QID = %d\n", qid);
	while(gold > 0){
		for(i = 0; i < worker; i++){
			msgrcv(qid, &buf, length, buf.mtype, 0);
			gold = gold - buf.one;
			gl[i] += buf.one;
			printf("Раб под номером: %d несет %d золота из рудника, остаток %d, результат раба: %d\n", buf.pid, buf.one, gold, gl[i]);
			if(gold <= 0){
				printf("Рудник обрушился\n");
				for (i = 0; i < 20; i++){
					kill(pid[i], SIGKILL);
					printf("Раб %d убит\n", pid[i]);
					printf("Раб под номером: %d принес %d золота до обрушения\n", pid[i], gl[i]); 	
				}
			}
		}	
	}	
	msgctl(qid, IPC_RMID, 0);
	return 0;
}