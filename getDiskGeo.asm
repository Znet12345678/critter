[bits 16]
section .data
ret:times 0x20 db 0
section .text
global _getDiskCX
global _getDiskDH
_getDiskCX:
push bp
mov bp,sp
mov ah,0x08
mov dl,[0x100]
int 0x13
mov ax,cx
pop bp
ret
_getDiskDH:push bp
mov bp,sp
mov ah,8
mov dl,[0x100]
int 0x13
mov dl,dh
xor dh,dh
mov ax,dx
pop bp
ret
