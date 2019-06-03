#include <stdio.h>
void putc(char c){
	asm("pushal");
	asm("mov %0,%%ebx" :  : "m"(c));
	asm("mov $0x8,%ah");
	asm("int $0x80");
	asm("popal");
}
