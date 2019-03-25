#include "lib.h"
void *malloc(unsigned short size){
#ifndef __PM
	struct Mem *pntr = (struct Mem*)0x9000;
#else
	struct Mem *pntr = (struct Mem*)0x01000000;
#endif
	void *ret;
	while(1){
#ifndef __PM
		if((pntr+size) < 0x9000){
#else
		if((pntr+size) < 0x01000000){
#endif
			puts("OUT OF MEMORY!!!!\n");
			panic();
		}
		if(pntr->alloc){
			pntr+=sizeof(*pntr)+pntr->size;
			continue;
		}
		if(pntr->size >= (size)){
			unsigned short save = pntr->size;
			pntr->size = size;
			pntr->alloc = 1;
		       	ret=pntr + sizeof(*pntr);
			pntr+=size;
			pntr->alloc = 0;
			pntr->size = save-size;
			return ret;
		}else if(pntr->size != 0){
			pntr+=pntr->size+sizeof(*pntr);
		}else{
			pntr->alloc = 1;
			pntr->size = size;
			return pntr + sizeof(*pntr);
		}
	}
}
void free(void *mem){
	struct Mem *pntr = (struct Mem*)mem;
	pntr-=sizeof(struct Mem*);
	pntr->alloc = 0;
}
