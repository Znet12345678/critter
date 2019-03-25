all:
	nasm -f bin pre.asm -o pre.bin
	nasm -f bin stage1.asm -o stage1.bin
	cat pre.bin stage1.bin > bootloader.bin
