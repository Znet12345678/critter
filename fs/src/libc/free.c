#include <stdlib.h>
void free(void *pntr){
	asm("pushal");
	asm("mov $0x7,%ah");
	asm("mov %0,%%ebx" : : "m"(pntr));
	asm("int $0x80");
	asm("popal");
}
