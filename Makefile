OBJS = objs/stage2.o main.o objs/ulcmp.o  objs/putc.o objs/elf.o objs/memcmp.o objs/gdt.o objs/memcpy.o  objs/getc.o objs/fat.o  objs/readdisk.o objs/string.o objs/malloc.o objs/puts.o objs/test.o objs/main.o objs/disk.o objs/getDiskGeo.o objs/bzero.o objs/writedisk.o
STAGE3_OBJS = stage3.o llfs.o tmpfs.o memcmp.o string.o puts.o math.o objs/tty.o ata.o malloc.o
LIBK_OBJS =  crt0.o cursor.o
MATH_OBJS = math.o
BINS = *.a *.bin *.elf *.img
ASM=nasm
PWD=$(shell pwd)
CC_16=bcc
CFLAGS_16=-I. -DBLBUILD -DBCC -D__INTERNAL
CFLAGS_X86_32=-nostdlib -ffreestanding -I. -DKERN -D__STANDALONE -Iinclude -std=gnu99
LD_16=ld86
LD_32=i386-elf-gcc
CFLAGS_HOST=-I.
CC_HOST=gcc
CC=i386-elf-gcc
CFLAGS_HOST=-I. -g
PREFIX=i386-elf-
all:
	@echo "(MKIMG) fs.img"
	@qemu-img create fs.img 10M
	@echo "(HOST_CC) llfs.inscribe"
	@${CC_HOST}  llfs.inscribe.c -o llfs.inscribe ${CFLAGS_HOST}
	@echo "(CLEAN) ."
	@rm -f *.o *.a *.so *.elf
	@dd if=/dev/zero of=testing.img bs=1M count=100
	@make -C stage1
	@echo "(AS) ${PWD}/ata.o"
	@${ASM} -f elf ata.asm -o ata.o 
	@echo "(CC) ${PWD}/objs/tty.o"
	@${CC}  -c tty.c -o objs/tty.o ${CFLAGS_X86_32}
	@echo "(AS) ${PWD}/objs/ulcmp.o"
	@${ASM} -f as86 ulcmp.asm -o objs/ulcmp.o
	@echo "(AS) ${PWD}/writedisk.o"
	@${ASM} -f as86 writedisk.asm -o objs/writedisk.o
	@echo "(AS) ${PWD}/objs/getDiskGeo.o"
	@${ASM} -f as86 getDiskGeo.asm -o objs/getDiskGeo.o
	@echo "(AS) ${PWD}/objs/gdt.o"
	@${ASM} -f as86 gdt.asm -o objs/gdt.o
	@echo "(CC 16bit) ${PWD}/objs/bzero.o"
	@${CC_16} -ansi -c bzero.c -o objs/bzero.o 
	@echo "(CC 16bit) ${PWD}/objs/tmpfs.o"
	@${CC_16} -ansi -c tmpfs.c -o objs/tmpfs.o ${CFLAGS_16}
	@echo "(CC 16bit) ${PWD}/main.o"
	@${CC_16} -c main.c -o main.o  -ansi -I. ${CFLAGS_16}
	@echo "(CC 16bit) ${PWD}/objs/string.o"
	@${CC_16} -ansi -c string.c -o objs/string.o ${CFLAGS_16}
	@echo "(AS) ${PWD}/objs/stage2.o"
	@${ASM} -f as86 stage2.asm -o objs/stage2.o
	@echo "(AS) ${PWD}/objs/main.o"
	@${ASM} -f as86 main.asm -o objs/main.o
	@echo "(AS) ${PWD}/objs/putc.o"
	@${ASM} -f as86 putc.asm -o objs/putc.o
	@echo "(AS) ${PWD}/objs/readdisk.o"
	@${ASM} -f as86 readdisk.asm -o objs/readdisk.o
	@echo "(CC 16bit) ${PWD}/objs/puts.o"
	@${CC_16} -ansi -c puts.c -o objs/puts.o ${CFLAGS_16}
	@echo "(AS) ${PWD}/objs/disk.o"
	@${ASM} -f as86 disk.asm -o objs/disk.o
	@echo "(CC 16bit) ${PWD}/objs/test.o"
	@${CC_16} -ansi -c  test.c -o objs/test.o ${CFLAGS_16}
	@echo "(CC 16bit) ${PWD}/objs/malloc.o"
	@${CC_16} -ansi -c malloc.c -o objs/malloc.o ${CFLAGS_16}
	@echo "(CC 16bit) ${PWD}/padding.bin"
	@${ASM} -f bin padding.asm -o padding.bin
	@echo "(CC) 16bit ${PWD}/objs/fat.o"
	@${CC_16} -ansi -c fat.c -o objs/fat.o ${CFLAGS_16}
	@echo "(AS) ${PWD}/objs/getc.o"
	@${ASM} -f as86 getc.asm -o objs/getc.o
	@echo "(CC 16bit) ${PWD}/objs/memcmp.o"
	@${CC_16} -ansi -c memcmp.c -o objs/memcmp.o ${CFLAGS_16}
	@echo "(CC 16bit) ${PWD}/objs/memcpy.o"
	@${CC_16} -ansi -c memcpy.c -o objs/memcpy.o ${CFLAGS_16}
	@echo "(CC 16bit) ${PWD}/objs/elf.o"
	@${CC_16} -ansi -c elf.c -o objs/elf.o -I. ${CFLAGS_16}
	@echo "(LD 16bit) ${PWD}/stage2.bin"
	@${LD_16} -T0x7000 ${OBJS} -o stage2.bin 
	@cat stage1/bootloader.bin stage2.bin > worm.aout.bin
	@echo "(AS) ${PWD}/test.o"
	@${ASM} -f elf test.asm -o test.o
	@echo "(AS) ${PWD}/sig.bin"
	@${ASM} -f bin sig.asm -o sig.bin
	@echo "(CC_HOST) pad"
	@gcc pad.c -o pad
	@echo "(PAD) worm.aout.bin"
	@./pad worm.aout.bin
	@echo "(LD) ${PWD}/test.elf"
	@${CC} -Ttext 0x10000 test.o -o test.elf -nostdlib -ffreestanding
	@echo "(AS) ${PWD}/stage2.2.o"
	@${ASM} -f elf stage2.2.asm -o stage2.2.o
	@echo "(CC) ${PWD}/elf.elf.o"
	@${CC} -c elf.c -o elf.elf.o -I. -nostdlib -ffreestanding ${CFLAGS_X86_32}
	@echo "(ASM) ${PWD}/readdisk.o"
	@${ASM} -f elf readdisk.asm -o readdisk.o
	@echo "(CC) ${PWD}/memcpy.o"
	@${CC} -c memcpy.c -o memcpy.o -nostdlib -ffreestanding -I. ${CFLAGS_X86_32}
	@echo "(CC) ${PWD}/memcmp.o"
	@${CC} -c memcmp.c -o memcmp.o ${CFLAGS_X86_32}
	@echo "(AS) ${PWD}/test.bin"
	@${ASM} -f bin test2.asm -o test2.bin 
	@echo "(LD) ${PWD}/stage2.2.elf"
	@${CC} stage2.2.o elf.elf.o readdisk.o memcpy.o memcmp.o -o stage2.2.elf -Ttext 0xF000 -nostdlib -ffreestanding
	@echo "(OBJCOPY) ${PWD}/stage2.2.bin"
	@objcopy -O binary --only-section=.text stage2.2.elf stage2.2.bin
	@echo "Padding binaries..."
	@./pad stage2.2.bin
	@./pad test2.bin
	@echo "****stage2 build complete****"
	@echo "(CC) ${PWD}string.o"
	@${CC} -c string.c -o string.o -nostdlib ${CFLAGS_X86_32} -D__PM
	@echo "(CC) ${PWD}/malloc.o"
	@${CC} -c malloc.c -o malloc.o -D__PM ${CFLAGS_X86_32} 
	@echo "(CC) ${PWD}/crt0.o"
	@${CC} -c crt0.c -o crt0.o
	@echo "(CC) ${PWD}/cursor.o"
	@${CC} -c cursor.c -o cursor.o ${CFLAGS_X86_32}
	@echo "(AR) ${PWD}/libk.a"
	@${PREFIX}ar rcs libk.a ${LIBK_OBJS}
	@echo "(CC) ${PWD}/math.o"
	@${CC} -c math.c -o math.o -I. ${CFLAGS_X86_32}
	@echo "(AR) ${PWD}/libm.a"
	@${PREFIX}ar rcs libm.a ${MATH_OBJS}
	@echo "(CC) ${PWD}/tmpfs.o"
	@${CC} -c tmpfs.c -o tmpfs.o ${CFLAGS_X86_32}
	@echo "(CC) ${PWD}/puts.o"
	@${CC} -c puts.c -o puts.o ${CFLAGS_X86_32} -D__PM
	@echo "(CC) ${PWD}/stage3.o"
	@${CC} -c stage3.c -o stage3.o ${CFLAGS_X86_32}
	@echo "(CC) ${PWD}/llfs.o"
	@${CC} -c llfs.c -o llfs.o ${CFLAGS_X86_32} -D__PM -Werror -D__FS_DEBUG
	@echo "(LD) ${PWD}/stage3.elf"
	@${CC} ${STAGE3_OBJS} -L. -lk -nostdlib -ffreestanding -o stage3.elf -Ttext 0x10000
	@cp *.bin *.elf objs/*.o fs/kern
	@echo "(INSCRIBE FILE SYSTEM) ${PWD}/fs.img"
	@./llfs.inscribe fs.img fs
	@echo "(CATBOOTDISK) ${PWD}/critter.img"
	@cat worm.aout.bin sig.bin stage2.2.bin stage3.elf padding.bin> critter.img
	@echo "(CC_HOST) ${PWD}/genimg.o"
	@gcc -c genimg.c -o genimg.o
	@echo "(LD_HOST) ${PWD}/genimg"
	@gcc genimg.o -o genimg
	@echo "(GENIMG) ${PWD}/critter.img"
	@./genimg critter.img fs.img
clean:
	@echo "(CLEAN) ${OBJS}"
	@rm -f ${OBJS}
	@echo "(CLEAN) ${STAGE3_OBJS}"
	@rm -f ${STAGE3_OBJS}
	@echo "(CLEAN) ${BINS}"
	@rm -f ${BINS}
