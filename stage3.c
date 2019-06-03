#include <llfs.h>
#include <lib.h>
#include <tmpfs.h>
#include <video.h>
#include <stdint.h>
void bzero(void *buf,unsigned int n){
	for(int i = 0; i < n;i++)
		*((uint8_t*)buf + n) = 0;
}
#define CPU "x86/i386 intel"
#define KVERSION "0.0.0-r0"
int main(){
	clear();
	puts("CPU Build Version: ");
	puts(CPU);
	puts("\nKernel ");
	puts(KVERSION);
	puts(" loading...");
	char *buf = malloc(1024);
	bzero(buf,1024);
	uint8_t slavebit = -1;
	uint16_t drive = -1;
	int8_t a = ata_read_master(buf,1,0x1f0,0xe0);
	if(a == 0){
		if(memcmp(buf,"WORM",4) == 0){
			drive = 0x1f0;
			slavebit = 0xe0;
			goto a;
		}
	}
	bzero(buf,512);
	a = ata_read_master(buf,1,0x1f0,0xf0);
	if(a == 0){
		if(memcmp(buf,"WORM",4) == 0){
			drive = 0x1f0;
			slavebit = 0xf0;
			goto a;
		}
	}
	bzero(buf,512);
	a = ata_read_master(buf,1,0x170,0xe0);
	if(a == 0){
		if(memcmp(buf,"WORM",4) == 0){
			drive = 0x170;
			slavebit = 0xe0;
			goto a;
		}
	}
	bzero(buf,512);

	a = ata_read_master(buf,1,0x170,0xf0);
	if(a == 0){
		if(memcmp(buf,"WORM",4) == 0){
			drive = 0x170;
			slavebit = 0xe0;
			goto a;
		}
	}
	puts("No devices Found!\n");
a:;
	*(uint16_t*)0x100 = drive;
	*(uint8_t *)0x102 = slavebit;
	free(buf);
	buf = malloc(512);
	ata_read_master(buf,0,*(uint16_t*)0x100,*(uint8_t*)0x102);
	*(uint32_t*)0x103 = *(uint32_t*)(buf + 0x1be);
	idt();
	const char *str = "Interupts work!\n";
	asm("mov %0,%%ebx" : :"m"(str));
	asm("mov $1,%ah");
	asm("int $0x80");
	char *arr[] = {"/init"};

	exec(arr[0],1,arr);
	panic("Nothing to do");
}
void panic(char *message){
	int address = 0;
	asm("movl 4(%ebp),%eax");
	asm("movl %%eax,%0" : "=m"(address));
	puts("0x");
	putx(address);
	puts(":panic(");
	puts(message);
	puts(")\nA fatal error has occured\n");
	asm("cli");
	asm("hlt");
}
