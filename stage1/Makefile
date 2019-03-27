all:
	@echo "(AS) pre.bin"
	@nasm -f bin pre.asm -o pre.bin
	@echo "(AS) stage1.bin"
	@nasm -f bin stage1.asm -o stage1.bin
	@echo "(CAT) bootloader.bin"
	@cat pre.bin stage1.bin > bootloader.bin
