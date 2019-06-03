#include "idt.h"
#include "mem.h"
#include "lib.h"
void idt(){
	struct IDTDescr *idt = (struct IDTDescr *)malloc(sizeof(*idt)*256);
	bzero(idt,sizeof(*idt)*256);
	idt[0x80].offset_1 = (uint16_t)0x15000;
	idt[0x80].offset_2 = 0x15000 >> 16;
	idt[0x80].selector = 0x8;
	idt[0x80].type_attr = 0b10001110;
	struct idt_info inf;
	inf.size = sizeof(*idt)*256-1;
	inf.pntr = idt;
	asm("lidt %0" : :"m"(inf));
}
