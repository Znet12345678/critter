#include "lib.h"
void puts(const char *str){
	unsigned char i = 0;
	while(*(str+i) > 0){
		putc(*(str + i));	
		i++;
	}
	return;
}
#ifdef __PM
int intlen(int i){
	int ret = 0;
	while(i > 0){
		i/=10;
		ret++;
	}
	return ret;
}
void puti(int i){
	int n = intlen(i)-1;
	int j = 0;
	while(i >= 0){
		putc(i/pow(10,intlen(i)-1)+'0');
		n--;
		j++;
		i-=(i/pow(10,intlen(i)-1))*pow(10,intlen(i)-1);
		if(i == 0)
			break;
	}

}
#endif
#ifdef __PM
void putb(uint8_t n){
	for(int i = 0; i < 8;i++){
		putc(((n >> (8-i)) & 1)+'0');
	}
	putc('\n');
}
#endif
