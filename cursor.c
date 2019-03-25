#include <stdint.h>
void outb(uint16_t p,uint8_t b){
	asm("outb %0,%1" :  : "a"(b), "Nd"(p));
}
void move_cursor(int x,int y){
	uint16_t pos = y * 80 + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
