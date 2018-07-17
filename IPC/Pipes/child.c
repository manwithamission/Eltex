#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <wait.h>


int main(int argc, char *argv[]){
	char goldstr[6];
	while(1) {
		sprintf(goldstr, "%d", 1);
		// printf("%s ЧТО ЭТО", argv[0]);
		write(atoi(argv[1]), goldstr, 2);
	}
}