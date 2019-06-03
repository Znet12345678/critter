#include <stdlib.h>
#include <stdint.h>
void bzero(void *pntr,unsigned long n){
	for(int i = 0; i < n;i++)
		*(uint8_t*)(pntr + i) = 0;
}
