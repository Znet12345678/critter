#ifndef __TMPFS_INTRNLS_H
#define __TMPFS_INTRNLS_H
#include <stdint.h>
struct tmpfs_FD{
	uint8_t alloc;
	uint8_t options;
	uint8_t c;
	uint8_t h;
	uint8_t s;
};
#endif
