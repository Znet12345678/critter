#ifndef __STDLIB_H
#define __STDLIB_H
#include <stdint.h>
void *malloc(unsigned long n);
void bzero(void *pntr,unsigned long n);
void exec(const char *path,int argc,char *argv[]);
void free(void *pntr);
struct Mem{
        uint8_t alloc;
        uint32_t size;
};

#endif
