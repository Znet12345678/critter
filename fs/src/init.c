#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc,char *argv[]){
	puts("CRITTER init\n");
	puts("Dropping into shell for now...\n");
	while(1){
		puts(">");
		char *command = malloc(1024);
		bzero(command,1024);
		strcpy(command,gets(STDIN));
		char **arr = sep(command,' ');
		int k = 0;
		while(arr[k] != 0)
			k++;

		char *path = malloc(1024);
		strcpy(path,"/usr/bin/");
		strcat(path,arr[0]);
		if(arr[0][0] == 0)
			goto b;
		exec(path,k,arr);
b:;		free(command);
		free(arr);
		puts("\n");
	}
	return 0;
}
