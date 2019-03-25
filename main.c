#include <lib.h>
#include <video.h>
void io_error(){
	puts("An I/O Error has occured! Trying to use bios calls in pmode?");
}
void pmain(){
	go_protected();
}
void main(){
	while(1);
}
void rmain(){
	char *memtest = malloc(1024);
	char *freeme = malloc(80);
	char *memtest2 = malloc(1024);
	void *buf;
	short ret;
	int fda = 0,fdb = 0,hda = 0,hdb = 0;
	strcpy(memtest,"Memory allocation ");
	free(freeme);
	strcpy(memtest2,"Works!\n");
	puts(memtest);
	puts(memtest2);
	while(1);
	puts("Probing Hard Drives\n");
	puts("FDA->");
	buf=malloc(1024);
	/*PNTR,DRIVE,HEAD,SECTOR,CYLINDER*/     
	ret = bios_readdisk(buf,0,0,1,0);
	if((ret >> 8) == 0){
		puts("FOUND\n");
		fda = 1;
	}else
		puts("ABSENT\n");
	puts("FDB->");
	ret = bios_readdisk(buf,1,0,1,0);
	if((ret >> 8) == 0){
		puts("FOUND\n");
		fdb = 1;
	}else
		puts("ABSENT\n");
	puts("HDA->");
	ret = bios_readdisk(buf,0x80,0,1,0);
	if((ret >> 8) == 0){
		puts("FOUND\n");
		hda = 1;
	}else
		puts("ABSENT\n");
	puts("HDB->");
	ret = bios_readdisk(buf,0x81,0,1,0);
	if((ret >> 8) == 0){
		puts("FOUND\n");
		hdb = 1;
	}else
		puts("ABSENT\n");
	puts("Installing worm...\n");
	free(buf);
	free(memtest);
	free(memtest2);
	while(1);
}
int pow(int a,int b){
	int ret = 1,i = 0;
	for(i = 1; i <= b;i++)
		ret*=a;
	return ret;
}
int atoh(char *buf){
	uint8_t dec = 0,i = 0;
	for(i = 0; i < strlen(buf);i++){
		if(buf[i] >= '0' && buf[i] <= '9'){
			dec+=(buf[i]-'0')*pow(16,strlen(buf)-i-1);
		}
		else{
			dec+=(buf[i]-'A'+10)*pow(16,strlen(buf)-i-1);
		}
	}
	return dec;
}

