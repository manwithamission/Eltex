#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *array[10];

int init_str() {	
	int strnum;
	printf("Введите кол-во строк: ");
	scanf("%d", &strnum);
	// printf("Кол-во строк: %d\n", strnum);
	int i;

	if (strnum > 10) {
		strnum = 10;
	}

	for (i = 0; i < strnum; i++) {
		array[i] = (char*)malloc(strnum + 1);
		printf("Введите строку: \n");
		scanf("%s", array[i]);
	};

	// for (i = 0; i < strnum; i++) {
	// 	printf("Строка: %s\n", array[i]);
	// };

	return strnum;
}

static int cmp(const void *p1, const void *p2){
	return strcmp(*(char* const*)p1, *(char* const*)p2);
}

int main() {
	int strnum;
	strnum = init_str();
	qsort(array, strnum, sizeof(char *), cmp);
	int i;
	for (i = 0; i < strnum; i++) {
		printf("%d: %s \n", i, array[i]);
		free(array[i]);
	}
	return 0;
}