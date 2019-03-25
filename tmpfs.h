#ifndef __TMPFS_H
#define __TMPFS_H
#include <stdint.h>
struct Entry_16{
	uint8_t alloc;
	uint16_t type;
	uint16_t data;
	uint8_t name[0x20];
	uint16_t sc;
	uint8_t sh;
	uint8_t ss;
	uint16_t ec;
	uint8_t eh;
	uint8_t es;
	uint16_t nc;/*TODO*/
	uint8_t nh;
	uint8_t ns;
	uint32_t offset;
};


int mkfs_tmpfs(int fd);
int read(int fd,void *buf,unsigned int n);
int write(int fd,void *buf,unsigned int n);
int open(const char *path,unsigned int flags);
#define O_RDONLY 0x1
#define O_RDWONLY 0x2
#define O_RDWR 0x4
#define T_FILE 0
#define T_DIR 1
#define T_DEV 2
#endif
