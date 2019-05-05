[bits 32]

section .data
errmsg:db 'I/O Error!',0xa,0
slavebyte:db 0
drive:dd 0
lba:dd 0
pntr:dd 0
sc:db 1
cnt:dw 0
msg:db 'Yo!',0xa,0
section .text
extern putc
extern panic
extern putb
extern puti
extern puts
global ata_read_master
ata_read_master:
push ebp
mov ebp,esp
mov edi,[ebp+8]
mov dword [pntr],edi
mov ebx,[ebp+12]
mov dword [lba],ebx
mov bx,[ebp+16]
mov word [drive],bx
mov bx,[ebp+20]
mov byte [slavebyte],bl
mov byte al,[lba+3]
and al,0x0f
or al,[slavebyte]
out byte dx,al
mov al,[sc]
mov dx,[drive]
add dx,2
out dx,al
mov al,[lba]
mov dx,[drive]
add dx,3
out byte dx,al
mov al,[lba+1]
mov dx,[drive]
add dx,4
out byte dx,al
mov al,[lba+2]
mov dx,[drive]
add dx,5
out byte dx,al
mov al,0x20
mov dx,[drive]
add dx,7
out byte dx,al
call wait_irq
mov dword edi,[pntr]
loop1:
mov dx,[drive]
in word ax,dx
mov word [edi],ax
add edi,2
inc word [cnt]
cmp word [cnt],512
jl loop1
pop ebp
ret
wait_400ns:
mov dx,[drive]
add dx,7
loop2:mov word [cnt],0
_loop2:
in byte ax,dx
cmp word [cnt],4
je loop2r
inc word [cnt]
jmp _loop2
loop2r:mov word [cnt],0
ret
wait_irq:
push ebp
mov ebp,esp
call wait_400ns
loop:
mov dx,[drive]
add dx,7
in byte al,dx
mov bl,al
xor ah,ah
shr al,7
and al,1
cmp al,0
jne loop
mov al,bl
shr al,3
and al,1
cmp al,1
jne loop
mov al,bl
and al,1
cmp al,1
je io_error
mov al,bl
shr al,5
cmp al,1
je l1
mov ax,0
pop ebp
ret
l1:call io_error
pop ebp
ret
io_error:
push ebp
mov ebp,esp
push errmsg
call puts
pop ebp
mov ax,-1
ret

