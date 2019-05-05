#include "mem.h"
uint32_t paged[1024] __attribute__((aligned(4096)));
uint32_t paget[1024] __attribute__((aligned(4096)));
int init_page(){
	for(int i = 0; i < 1024;i++){
		paged[i] = 2;
	}
}
