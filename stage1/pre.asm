[bits 16]
[org 0x7c00]
jmp _start
sig:db 'WORM'
msg:db "Looking for boot disk...",0
jmping:db 'Jumping...',0
errmsg:db "Error!",0
cmpring:db "Comparing...",0
start:mov bx,sig
puts_bios:
mov ah,0x0e
lbl:
mov al,[bx]
cmp al,0
je r
int 0x10
inc bx
jmp lbl
r:ret
_start:
mov bx,msg
call puts_bios
mov ax,ds
mov es,ax
mov ah,0x02
mov al,2
mov ch,0
mov cl,2
mov dh,0
mov dl,0
mov bx,0x1000
a:
int 13h
cmp ah,0
je cmp1
mov ah,0
mov ah,2
mov al,2
cmp dl,0x82
je err
inc dl
jmp a
cmp1:
push bx
mov bx,cmpring
call puts_bios
pop bx
cmp  byte [0x1000],'W'
jne a
cmp byte [0x1001],'O'
jne a
cmp byte [0x1002],'R'
jne a
cmp byte [0x1003],'M'
jne a
jmp jmplbl
jmp a
jmplbl:mov bx,jmping
call puts_bios
mov byte [0x50],dl
jmp 0:0x1004
err:mov bx,errmsg
call puts_bios
hng:
jmp hng
times 510 - ($ - $$) db 0
dw 0xaa55
