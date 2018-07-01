#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct bo {
	char name[50];
	int year;
	int pagenum;
}; 
typedef struct bo books;

void readBook(books *bk) {
	printf("Введите название книги: \n");
	scanf("%s", (*bk).name);
	printf("Введите год издания: \n");
	scanf("%d", &(*bk).year);
	printf("Введите количество страниц: \n");
	scanf("%d", &(*bk).pagenum);
}

static int cmp(const void *p1, const void *p2){
	return strcmp(*(char* const*)p1, *(char* const*)p2);
}

int main(){
	int booksum;
	printf("Введите кол-во книг: ");
	scanf("%d", &booksum);
	int i;
	books** bk;
	bk = (books**) malloc (sizeof(books*) * booksum);

	for (i = 0; i < booksum; i++) {
		bk[i] = (books*) malloc (sizeof(books));
		readBook(bk[i]);
	};

	qsort(bk, booksum, sizeof(books*), cmp);

	for (i = 0; i < booksum; i++) {
		printf("Название книги: %s\t", (*bk[i]).name);
		printf("Год издания: %d\t", (*bk[i]).year);
		printf("Кол-во страниц: %d\t\n", (*bk[i]).pagenum);
		free(bk[i]);
	}
    free(bk);

	return 0;
}