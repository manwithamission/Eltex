#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int i, *gold, worker, status, stat;
key_t key;
int *gl;
int shmid, semid;
struct sembuf lock_res = {0, -1, 0};	//блокировка ресурса
struct sembuf rel_res = {0, 1, 0};		//освобождение ресурса

int main(int argc, char *argv[]) {
	key = ftok(".", 'm');
	printf("Укажите кол-во рабочих:");
	scanf("%d", &worker);
	int pid[worker];

	gl = (int*)malloc(worker * sizeof(int)); 									//выделение памяти

	for (i = 0; i < worker; i++) {												//инициализация массива рабочих
		gl[i] = 0;
	}

	if((shmid = shmget(key, sizeof(int), IPC_CREAT | 0666)) < 0) {				//создаем разделяемую память
		perror("shmget");
		exit(1);
	}

	semid = semget(key, 1, 0666 | IPC_CREAT);				//создаем семафор
	semctl(semid, 0, SETVAL, 1);

	if((gold = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) { 			//получаем доступ к разделяемой памяти
		printf("Can't attach shared memory\n");
		exit(-1);
	}

	semop(semid, &lock_res, 1); 										//блокируем разделяемую память

	printf("Установите кол-во золота в руднике:");						//установка стартового кол-ва золота
	scanf("%d", gold);
	// printf("Кол-во золота: %d\n", *gold);

	semop(semid, &rel_res, 1);											//освобождаем разделяемую память

	for(i = 0; i < worker; i++) {
		pid[i]=fork();
		if(pid[i] == 0) {
			// printf("SON - QID = %d\n", qid);
			while(*gold > 0) {
				semop(semid, &lock_res, 1); 										//блокируем разделяемую память

				*gold = *gold - 1;
				gl[i] += 1;
				printf("Раб под номером: %d несет %d золота из рудника, остаток %d, результат раба: %d\n", getpid(), 1, *gold, gl[i]);
				if(*gold <= 0){
					printf("Рудник обрушился\n");
					semop(semid, &rel_res, 1);	
					break;
				}

				semop(semid, &rel_res, 1);											//освобождаем разделяемую память
				sleep(1);
			}
		}
	}
	wait(NULL);
//	for (i = 1; i < worker; i++) {
//		status = waitpid(pid[i], &stat, 0);
//		if (pid[i] == status) {
//			printf("процесс-потомок %d done,  result=%d, золота принес: %d\n", pid[i], WEXITSTATUS(stat), gl[i]);
//		}
//	}

	shmctl(shmid, IPC_RMID, 0);
	semctl(semid, 0, IPC_RMID);
	return 0;
}