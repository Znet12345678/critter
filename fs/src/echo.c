#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
//	putx(argc);
	for(int i = 1; i < argc;i++){
		puts(argv[i]);
		puts(" ");
	}
	return 0;
}
