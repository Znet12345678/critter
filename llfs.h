#ifndef __LLFS_H
#define __LLFS_H
#include <stdint.h>
struct Entry{
	uint8_t type;
	uint32_t nxtlba;
	uint32_t contlba;
	uint32_t size;
	uint8_t nsize;
}__attribute__((packed));
#define TYPE_DIR 1
#define TYPE_FILE 2
#define TYPE_EMPTY 3
#ifdef KERN_BUILD
int llread(int fd,void *buf,int n);
#endif
#endif
