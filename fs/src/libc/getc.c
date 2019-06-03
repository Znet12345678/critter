#include <stdio.h>
char getc(int fd){
	asm("pushal");
	asm("mov $0x6,%ah");
	asm("mov %0,%%ebx" : : "m"(fd));
	asm("int $0x80");
	char c;
	asm("movb %%eax,%0" : "=m"(c));
	asm("popal");
	return c;
}
