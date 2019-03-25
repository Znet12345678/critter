[bits 16]
global _putc
_putc:
push bp
mov bp,sp
mov ax,[bp+4]
cmp al,0xa
je nl
mov ah,0x0e
int 0x10
o:
pop bp
ret
nl:
mov ah,3
mov bh,0
int 0x10
mov ah,2
inc dh
mov dl,0
int 0x10
jmp o
