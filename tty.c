#include <stdint.h>
uint8_t r = 0,c = 0;
#define COLOR_BLACK BLACK
#define COLOR_WHITE WHITE
#include <video.h>
#include <lib.h>
char *vgabuf = (char*)0xb8000;
uint8_t mkcolor(uint8_t a,uint8_t b){
	return a | b << 4;
}
void clear(){
	int i = 0;
	for(i = 0; i < 80*25;i++)
		*(uint16_t*)(vgabuf + i*2) = ' ' | mkcolor(COLOR_WHITE,COLOR_BLACK)  << 8;
	r = 0; c = 0;
}
void putc(char ch){
	if(ch != '\n')
		*(uint16_t *)(vgabuf+(c+r*80)*2) = ch | mkcolor(COLOR_WHITE,COLOR_BLACK) << 8;
	c++;
	if(c == 80 || ch == '\n'){
		r++; 
		c = 0;
	}
	if(r == 25){
		scroll();
		r--;
	}
	move_cursor(c,r);
}
void scroll(){
	for(int r = 1; r <= 25; r++){
		for(int c = 0; c < 80;c++){
			vgabuf[((r-1)*80+c)*2] = vgabuf[(r*80+c)*2];
		}
	}
}
