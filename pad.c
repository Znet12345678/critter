#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc,char *argv[]){
	if(argc != 2)
		return -1;
	FILE *f = fopen(argv[1],"rb");
	FILE*f2 = fopen(argv[1],"ab");
	if(!f || !f2)
		return -1;
	fseek(f,0,SEEK_END);
	int size = ftell(f);
	fclose(f);
	for(int i = 0; i < 512-size%512;i++)
		fputc(0,f2);
	fclose(f2);
	return 0;
}
