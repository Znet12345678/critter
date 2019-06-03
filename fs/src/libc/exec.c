#include <stdlib.h>
void exec(const char *path,int argc,char *argv[]){
	asm("pushal");
	asm("mov $0x5,%ah");
	asm("mov %0,%%ebx" : :"m"(path));
	asm("mov %0,%%ecx" : :"m"(argc));
	asm("mov %0,%%edx" : :"m"(argv));
	asm("int $0x80");
	asm("popal");
}
