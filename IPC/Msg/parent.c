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
int i, pid[20], gold;
int qid;
key_t msgkey;
int gl[20] = {0,0,0,0,0,0};
int length;

int main(int argc, char *argv[]) {
	length = sizeof(mess_t) - sizeof(long);
	msgkey = ftok("~labs/Eltexlabs/Clabs/msgget/foo", 42);
	qid = msgget(msgkey, IPC_CREAT | 0660);
	printf("Стартовое количество золота в руднике:");
	scanf("%d", &gold);
	for(i = 0; i < 20; i++) {
		pid[i]=fork();
		if(pid[i] == 0) {
			printf("SON - QID = %d\n", qid);
			while(1) {
				buf.mtype = 1;
				buf.one = 1;
				buf.pid = getpid();
				msgsnd(qid, &buf, length, 1);
			}
		}
	}
	printf("FATHER - QID = %d\n", qid);
	while(gold > 0){
		for(i = 0; i < 20; i++){
			msgrcv(qid, &buf, length, buf.mtype, 0);
			gold = gold - buf.one;
			gl[i] += buf.one;
			printf("Раб под номером: %d несет %d золота из рудника, остаток %d\n", buf.pid, buf.one, gold);
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