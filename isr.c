#include "lib.h"
asm(".section .isr");
void isr_main(){
#ifdef DEBUG_INT
	puts("int 0x80,0x");
	int s;
	asm("mov %ah,%0" : "=m"(s));
	putx(s);
#endif
}
