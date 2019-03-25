[bits 16]
bios_read_sector:
pop si
mov ax,ds
mov es,ax
mov ah,0x02
mov al,1
pop cx
pop dx
pop bx
int 0x13
jmp si
