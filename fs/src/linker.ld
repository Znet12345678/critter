ENTRY(_start)
SECTIONS{
	. = 10M;
	.TEXT BLOCK(4K):ALIGN(4K){
                *(.text)
	}
}
