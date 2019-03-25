section .text
global _getc
extern _putc
_getc:push bp
mov bp,sp
mov ah,0
int 0x16
push ax
call _putc
pop ax
pop bp
ret
