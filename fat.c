#include "lib.h"
#include "fat.h"
#include "mbr.h"
void fat_parse(unsigned char drive){
	struct Important_Values *vals = parse_important(drive);	
}
parse_important(unsigned char drive){
	struct Important_Values *ret = malloc(sizeof(*ret));
	unsigned char *bootsector;
	int i = 0;
	puts("Searching for fat...\n");
	bootsector = malloc(512);
	bios_readdisk(bootsector,drive,0,1,0);
	for(i = 0; i < 4;i++){
		struct PartTab * ptab = (struct PartTab *)(bootsector + i*0x10);
		unsigned short cs = *(unsigned short *)(ptab->scs);
		unsigned char head = *(unsigned char *)(ptab->shead);
		struct fat_BS *bs;
		unsigned char *buf = malloc(512);
		struct fat_extBS_16 *fat16;
		bios_readdisk(buf,drive,head,cs,cs >> 8);
		bs = (struct fat_BS*)buf;
		fat16 = (struct fat_extBS_16 *)(buf + sizeof(struct fat_BS));
		if(*(buf + 38) != 0x28 && *(buf+38) != 0x29){
			puts("On volume ");
			putc(i + '0');
			puts(": FAT not found\n");	
		}

	}
}
