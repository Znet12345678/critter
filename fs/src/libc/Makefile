OBJS=puts.o crt0.o malloc.o exec.o crti.o crtn.o  putc.o sep.o bzero.o putx.o string.o gets.o getc.o free.o
LIBS=libc.a
CC=i386-elf-gcc
AS=nasm
AR=i386-elf-ar
CFLAGS=-Iinclude -nostdlib -ffreestanding
all:
	@rm -f *.a *.o
	@echo "(AS) ${PWD}/crti.o"
	@${AS} -f elf crti.asm -o crti.o
	@echo "(AS) ${PWD}/crtn.o"
	@${AS} -f elf crtn.asm -o crtn.o
	@echo "(CC) ${PWD}/sep.o"
	@${CC} -c sep.c -o sep.o ${CFLAGS}
	@echo "(CC) ${PWD}/string.o"
	@${CC} -c string.c -o string.o ${CFLAGS}
	@echo "(CC) ${PWD}/puts.o"
	@${CC} -c puts.c -o puts.o ${CFLAGS}
	@echo "(CC) ${PWD}/bzero.o"
	@${CC} -c bzero.c -o bzero.o ${CFLAGS}
	@echo "(CC) ${PWD}/crt0.o"
	@${CC} -c  crt0.c -o crt0.o ${CFLAGS}  -Werror
	@echo "(CC) ${PWD}/gets.o"
	@${CC} -c gets.c -o gets.o ${CFLAGS}
	@echo "(CC) ${PWD}/getc.o"
	@${CC} -c getc.c -o getc.o ${CFLAGS}
	@echo "(CC) ${PWD}/free.o"
	@${CC} -c free.c -o free.o ${CFLAGS}
	@echo "(CC) ${PWD}/malloc.o"
	@${CC} -c malloc.c -o malloc.o ${CFLAGS}
	@echo "(CC) ${PWD}/exec.o"
	@${CC} -c exec.c -o exec.o ${CFLAGS}
	@echo "(CC) ${PWD}/putc.o"
	@${CC} -c putc.c -o putc.o ${CFLAGS}
	@echo "(CC) ${PWD}/putx.o"
	@${CC} -c putx.c -o putx.o ${CFLAGS}
	@echo "(AR) ${PWD}/libc.a"
	
	@${AR} rcs libc.a ${OBJS}
install:
	cp crt0.o /usr/local/lib/i386-elf
	cp libc.a /usr/local/lib
