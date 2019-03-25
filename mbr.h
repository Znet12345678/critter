#ifndef __MBR_H
#define __MBR_H
#define PART_SIZE 0x10
#define FIRST_PART 0x1BE
#define SECOND_PART FIRST_PART + PART_SIZE
#define THIRD_PART SECOND_PART + PART_SIZE
#define FOURTH_PATH THIRD_PART + PART_SIZE
#ifndef BCC
#include <stdint.h>
#else
#include "_stdint.h"
#endif
struct PartTab{
	uint8_t bootable;
	uint8_t shead;
	uint16_t scs;
	uint8_t id;
	uint8_t ehead;
	uint16_t ecs;
	uint32_t lba;
	uint32_t tsectors;
};
#endif
