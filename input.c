#include "lib.h"
static int sca[] = {0x1e,0x30,0x2e,0x20,0x12,0x21,0x22,0x23,0x17,0x24,0x25,0x26,0x32,0x31,0x18,0x19,0x10,0x13,0x1f,0x14,0x16,0x2f,0x11,0x2d,0x15,0x2c};
int getIndx(int c){
	for(int i = 0; i < 26;i++)
		if(c == sca[i])
			return i;
	return -1;
}
int kbd_init(){
}

char getc(int fd){
	if(fd == STDIN){
a:;		int b = inb(0x60);
		int prev = b;
		while(!(inb(0x64) & 1)){
			continue;
		}
		if(getIndx(b) >= 0){
			putc(getIndx(b)+'a');

			return getIndx(b)+'a';
		}
		else if(b < 0x0b){
			putc(b-1+'0');
			return b-1+'0';
		}
		else if(b == 0x0b){
			putc('0');
			return '0';
		}
		else if(b == 0x1c){
			return '\n';
		}else if(b == 0x39){
			putc(' ');
			return ' ';
		}else if(b == 0x35){
			putc('/');
			return '/';
		}
		else
			goto a;
	}
	return -1;
}
char dummyfunc(){
	int a = getc(STDIN);
	return a;
}
