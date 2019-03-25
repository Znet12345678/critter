#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
int main(int argc,char *argv[]){
	if(argc != 3)
		return -1;
	FILE *f = fopen(argv[1],"r+b");
	if(!f)
		return -1;
	fseek(f,0,SEEK_END);
	int size = ftell(f);
	fseek(f,0,SEEK_SET);
	uint8_t *buf = malloc(512);
	fread(buf,1,512,f);
	*(uint32_t*)(buf +0x1be) = (uint32_t)(size/512)+1;
	fseek(f,0,SEEK_SET);
	fwrite(buf,1,512,f);
	FILE *f2 = fopen(argv[2],"rb");
	fseek(f,0,SEEK_END);
	while((ftell(f) % 512) != 0)
		putc(0,f);
	if(!f2)
		return -1;
	int c;
	while((c = getc(f2)) != EOF)
		putc(c,f);
	fclose(f);
	fclose(f2);
	return 0;
}
