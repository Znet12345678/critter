#include <stdio.h>
void putx(int c){
	asm("pushal");
	asm("mov %0,%%ebx" : : "m"(c));
	asm("mov $0x9,%ah");
	asm("int $0x80");
	asm("popal");
}
