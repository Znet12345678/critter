#ifndef __LIB_H
#define __LIB_H
#ifndef BCC
#include <stdint.h>
#else
#include "_stdint.h"
#endif
#define O_RDONLY 1
#define O_WRONLY 2
#define O_RDWR 3
extern void putc(char c);
extern void puts(const char *str);
void bzero(void *pntr,unsigned int n);
#ifdef __PM
unsigned int strlen(const char *str);
void *memcpy(void *dest,const void *src,unsigned int n);
char *strcpy(char *dest,const char *src);
int strcmp(const char *str,const char *str1);
#endif
void panic();
int pow(int a,int b);
struct Mem{
	uint8_t alloc;
	uint16_t size;
};
struct CHS{
	uint16_t c;
	uint16_t h;
	uint8_t s;
};
struct llfd{
	struct Entry *pntr;
	struct llfd *nxt;
	uint8_t t;
};
struct llfd *getAddressSpace();
int ata_read_master(void *buf,unsigned int lba,unsigned short drive,unsigned char slavebyte);
short bios_readdisk(void *pntr,uint8_t drive,uint8_t head,uint8_t sector,uint8_t cylinder);
void _bios_readdisk(void *pntr,uint8_t drive,uint8_t tsect,uint8_t c,uint8_t h,uint8_t s);
#endif
