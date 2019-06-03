#include <lib.h>
#include <stdint.h>
#include <elf.h>
#include <llfs.h>
#include <mem.h>
#include <string.h>
void exec(const char *path,int argc, char *argv[]){
#ifdef DEBUG
	puts("exec(");
	puts(path);
	puts(")");
#endif
	int fd = open(path,O_RDWR);
	if(fd < 0)
		return;
	uint8_t *buf = malloc(fsize(fd));
	int r = llread(fd,buf,fsize(fd));
	Elf32_Ehdr *ehdr = (Elf32_Ehdr *)buf;
	Elf32_Phdr *phdr = (Elf32_Phdr *)(buf + ehdr->e_phoff);
	for(int i = 0; i < ehdr->e_phnum;i++){
		memcpy((void*)phdr[i].p_paddr,buf + phdr[i].p_offset,phdr[i].p_memsz);
#ifdef DEBUG
		puts("{0x");
		putx((int)(buf + phdr[i].p_offset));
		puts("->0x");
		putx(phdr[i].p_vaddr);
		puts("}.");
		putx(phdr[i].p_memsz);
		puts("\n");
#endif
	}
	void (*func)()= (void (*))ehdr->e_entry;
#ifdef DEBUG
	puts("(");
	puts(path);
	puts(")");
	puts("->0x");
	putx(ehdr->e_entry);

	puts("\n");
#endif
	void **pntrarr = (void**)0x1000;
	*(uint8_t*)0x999 = 2;
	*(uint32_t*)0x1000 = argc;
	*(uint32_t*)0x1004 = (uint32_t)argv;
	func();
}
