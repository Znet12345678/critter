OBJS = objs/stage2.o main.o objs/ulcmp.o  objs/putc.o objs/elf.o objs/memcmp.o objs/gdt.o objs/memcpy.o  objs/getc.o objs/fat.o  objs/readdisk.o objs/string.o objs/malloc.o objs/puts.o objs/test.o objs/main.o objs/disk.o objs/getDiskGeo.o objs/bzero.o objs/writedisk.o
STAGE3_OBJS = stage3.o llfs.o tmpfs.o memcmp.o string.o puts.o math.o objs/tty.o ata.o malloc.o
LIBK_OBJS =  crt0.o cursor.o
MATH_OBJS = math.o
BINS = *.a *.bin *.elf *.img
all:
	qemu-img create fs.img 10M
	gcc  llfs.inscribe.c -o llfs.inscribe -I. -g
	rm -f *.
	dd if=/dev/zero of=testing.img bs=1M count=100
	make -C stage1
	nasm -f elf ata.asm -o ata.o 
	i386-elf-gcc -nostdlib -ffreestanding  -c tty.c -o objs/tty.o -I. -std=gnu99
	nasm -f as86 ulcmp.asm -o objs/ulcmp.o	
	nasm -f as86 writedisk.asm -o objs/writedisk.o
	nasm -f as86 getDiskGeo.asm -o objs/getDiskGeo.o
	nasm -f as86 gdt.asm -o objs/gdt.o
	bcc -ansi -c bzero.c -o objs/bzero.o 
	bcc -ansi -c tmpfs.c -o objs/tmpfs.o -D__INTERNAL
	bcc -c main.c -o main.o  -ansi -I. -DBCC
	bcc -ansi -c string.c -o objs/string.o -DBCC -I. 
	nasm -f as86 stage2.asm -o objs/stage2.o
	nasm -f as86 main.asm -o objs/main.o
	nasm -f as86 putc.asm -o objs/putc.o
	nasm -f as86 readdisk.asm -o objs/readdisk.o
	bcc -ansi -c puts.c -o objs/puts.o -DBCC
	nasm -f as86 disk.asm -o objs/disk.o
	bcc -ansi -c  test.c -o objs/test.o -DBCC
	bcc -ansi -c malloc.c -o objs/malloc.o -DBCC
	nasm -f bin padding.asm -o padding.bin
	bcc -ansi -c fat.c -o objs/fat.o -DBCC
	nasm -f as86 getc.asm -o objs/getc.o
	bcc -ansi -c memcmp.c -o objs/memcmp.o -DBCC
	bcc -ansi -c memcpy.c -o objs/memcpy.o -DBCC
	bcc -ansi -c elf.c -o objs/elf.o -I. -DBCC
	ld86 -T0x7000 ${OBJS} -o stage2.bin 
	cat stage1/bootloader.bin stage2.bin > worm.aout.bin
	nasm -f elf test.asm -o test.o
	nasm -f bin sig.asm -o sig.bin
	gcc pad.c -o pad
	./pad worm.aout.bin
	i386-elf-gcc -Ttext 0x10000 test.o -o test.elf -nostdlib -ffreestanding
	nasm -f elf stage2.2.asm -o stage2.2.o
	i386-elf-gcc -c elf.c -o elf.elf.o -I. -nostdlib -ffreestanding -D__STANDALONE
	nasm -f elf readdisk.asm -o readdisk.o
	i386-elf-gcc -c memcpy.c -o memcpy.o -nostdlib -ffreestanding -I. -D__STANDALONE
	i386-elf-gcc -c memcmp.c -o memcmp.o -nostdlib -ffreestanding -I. -D__STANDALONE
	nasm -f bin test2.asm -o test2.bin
	i386-elf-gcc stage2.2.o elf.elf.o readdisk.o memcpy.o memcmp.o -o stage2.2.elf -Ttext 0xF000 -nostdlib -ffreestanding
	objcopy -O binary --only-section=.text stage2.2.elf stage2.2.bin
	./pad stage2.2.bin
	./pad test2.bin
	i386-elf-gcc -c string.c -o string.o -nostdlib -D__PM -ffreestanding 
	i386-elf-gcc -c malloc.c -o malloc.o -D__PM -I. -nostdlib -ffreestanding
	i386-elf-gcc -c crt0.c -o crt0.o
	i386-elf-gcc -c cursor.c -o cursor.o -ffreestanding -nostdlib
	i386-elf-ar rcs libk.a ${LIBK_OBJS}
	i386-elf-gcc -c math.c -o math.o -I. -nostdlib -ffreestanding
	i386-elf-ar rcs libm.a ${MATH_OBJS}
	i386-elf-gcc -c tmpfs.c -o tmpfs.o -nostdlib -ffreestanding
	i386-elf-gcc -c puts.c -o puts.o -I. -nostdlib -ffreestanding -D__PM
	i386-elf-gcc -c stage3.c -o stage3.o -I. -nostdlib -ffreestanding -std=gnu99
	i386-elf-gcc -c llfs.c -o llfs.o -I. -nostdlib -ffreestanding -std=gnu99 -D__PM -Werror -D__FS_DEBUG
	i386-elf-gcc ${STAGE3_OBJS} -L. -lk -nostdlib -ffreestanding -o stage3.elf -Ttext 0x10000
	cp *.bin *.elf objs/*.o fs/kern
	./llfs.inscribe fs.img fs
	cat worm.aout.bin sig.bin stage2.2.bin stage3.elf padding.bin> critter.img
	gcc -c genimg.c -o genimg.o
	gcc genimg.o -o genimg
	./genimg critter.img fs.img
clean:
	rm -f ${OBJS}
	rm -f ${STAGE3_OBJS}
	rm -f ${BINS}
