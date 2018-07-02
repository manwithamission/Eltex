#include "stdio.h"
#include "stdlib.h"
#include "string.h"

char str[255];
FILE *fp, *fr;

int main(int argc, char* argv[]){
	fp = fopen(argv[1], "r");
	fr = fopen(argv[2], "r+w");
	int i;
	char *word;

	for (i = 0; i < 6; i++) {
		fgets(str, 255, fp);
		word = strstr(str, (char*) argv[3]);
		// printf("%s: %s\n", str, (char*) argv[3]);
		if (word == 0) {
			printf("%s\n", str);
			fprintf(fr, "%s", str);
		}
	}

	fclose(fp);
	return 0;
}