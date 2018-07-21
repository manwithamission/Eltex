#include <pthread.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <wait.h> 
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int i, worker, result, sum;
static int gold;

typedef struct use_mutex_tag {
	pthread_mutex_t mutex;
} use_mutex_t;

void *thread_func(void *args) {
	use_mutex_t *arg = (use_mutex_t*) args;
    pthread_mutex_lock(&(arg->mutex));
	if (gold <= 0) {
		printf("Рудник обрушился\n");
		pthread_exit(NULL);
	}
	int tmpgold = gold;
	tmpgold = tmpgold - 1;
	gold = tmpgold;
	printf("Раб под номером: %d несет %d золота из рудника, остаток %d\n", getpid(), 1, gold);
	pthread_mutex_unlock(&(arg->mutex));
	return NULL;
}

int main(int argc, char *argv[]) {
	printf("Укажите кол-во рабочих:");
	scanf("%d", &sum);
	pthread_t workers[sum];
	void *status[sum];

	printf("Установите кол-во золота в руднике:");						//установка стартового кол-ва золота
	scanf("%d", &gold);
	use_mutex_t param;
	pthread_mutex_init(&(param.mutex), NULL);

	for(i = 0; i < sum; i++) {
		result = pthread_create(&workers[i], NULL, thread_func, &param);
		if (result != 0) {
			perror("Error creating the first thread");
			return EXIT_FAILURE;
		}
	}

	for (int i = 0; i < sum; i++){
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