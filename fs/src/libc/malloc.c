#include <stdlib.h>
void *malloc(unsigned long size){
	asm("pushal");
	asm("mov $0x4,%ah");
	asm("mov %0,%%ebx" : : "m"(size));
	asm("int $0x80");
	void *ret;
	asm("mov %%eax,%0" : "=m"(ret));
	asm("popal");
	return ret;
}
