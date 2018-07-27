#include <pthread.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <wait.h> 
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "string.h"

int i, worker, result, sum;
static int gold;

typedef struct use_mutex_tag {
	pthread_mutex_t mutex;
	int counter;
	char* inputfile;
	char* outputfile;
	char* str;
} use_mutex_t;

void *thread_func(void *args) {
	use_mutex_t *arg = (use_mutex_t*) args;
	pthread_mutex_lock(&(arg->mutex));
	char pipename[] = "Поток номер:";
	FILE *fp;
	char tmpi[50];
	sprintf(tmpi, "%d", arg->counter);
	strcat(pipename, tmpi);
	strcat(pipename, "\n");
	fp = fopen(arg->outputfile, "ab");
	fputs(pipename, fp);
	fclose(fp);

	int run_time, find_result = 0;
	int line_num = 1;
	time_t start;
	char temp[512], findbuf[5], linenumbuf[5], tmpnumbers[50];
	char tmpstr[] = "Кол-во совпадений:";
	char tmpstr2[] = "Номера строк:";
	char* str = arg->str;

	start = time(NULL);
	run_time = time(NULL) - start;
	// /home/arthur/Eltex/Opensource/Lab01/init.txt
	fp = fopen(arg->inputfile, "r");
	int i = 1;

	while(fgets(temp, 512, fp) != NULL) {
		if((strstr(temp, str)) != NULL) {
			start = time(NULL);

			sprintf(tmpnumbers, "%d", line_num);
			strcat(linenumbuf, tmpnumbers);
			strcat(linenumbuf, ",");

			printf("Номер строки: %d\n", line_num);
			printf("\n%s\n", temp);
			find_result++;
		}
		line_num++;
	}
	run_time = time(NULL) - start;
	printf("Runtime: %d\n", run_time);

	if(fp) {
		fclose(fp);
	}
	// /home/arthur/Eltex/Opensource/Lab01/output.txt
	fp = fopen(arg->outputfile, "ab");
	sprintf(findbuf, "%d", find_result);
	strcat(tmpstr, findbuf);
	strcat(tmpstr, "\n");
	fputs(tmpstr, fp);

	strcat(tmpstr2, linenumbuf);
	strcat(tmpstr2, "\n");
	fputs(tmpstr2, fp);

	fclose(fp);

	pthread_mutex_unlock(&(arg->mutex));
	return NULL;
}

int main(int argc, char *argv[]) {
	// printf("Введите кол-во процессов/потоков:");
	// scanf("%d", &sum);
	pthread_t workers[atoi(argv[2])];
	void *status[atoi(argv[2])];

	use_mutex_t param;
	pthread_mutex_init(&(param.mutex), NULL);

	for(i = 0; i < atoi(argv[2]); i++) {
		param.counter = 1;
		param.inputfile = argv[4];
		param.outputfile = argv[6];
		param.str = argv[8];

		result = pthread_create(&workers[i], NULL, thread_func, &param);
		if (result != 0) {
			perror("Error creating the first thread");
			return EXIT_FAILURE;
		}
		param.counter++;
	}

	for (int i = 0; i < atoi(argv[2]); i++){
		result = pthread_join(workers[i], &status[i]);
		if (result != 0) {
			perror("Error joining the first thread");
			return EXIT_FAILURE;
		}
		free(status[i]);
	}

	pthread_mutex_destroy(&(param.mutex));
	return 0;
}