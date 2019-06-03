#include <string.h>
char **sep(const char *str, char c){
	asm("pushal");
	asm("mov %0,%%ebx" : : "m"(str));
	asm("mov %0,%%ecx" : : "m"(c));
	asm("mov $0xa,%ah");
	asm("int $0x80");
	char **ret;
	asm("mov %%eax,%0" :"=m"(ret));
	asm("popal");
	return ret;
}
