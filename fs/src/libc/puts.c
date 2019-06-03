#include <stdio.h>
void puts(const char *str){
	asm("pushal");
	asm("mov $1,%ah");
	asm("mov %0,%%ebx" : :"m"(str));
	asm("int $0x80");
	asm("popal");

}
