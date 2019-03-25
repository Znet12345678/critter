[bits 16]
[org 0x1000]
sig:db 'WORM'
jmp start
jmpe:db 'Jumping to entry...',0
ldelf:db 'Loading elf...',0
returnedmsg:db 'Returned to stage1! This should not happen!',0
errmsg:db 'Error!',0
loading:db 'Loading a.out into memory from disk...',0
succ:db '*',0
start:mov bx,succ

call puts_bios
jmp _start
puts_bios:
mov ah,0x0e
loop:
mov al,[bx]
int 0x10
inc bx
cmp byte [bx],0
je r
jmp loop
r:ret
aout_hdr_v7:
a_magic:dw 0
a_tsize:dw 0
a_dsize:dw 0
a_ssize:dw 0
a_entry:dw 0
a_pad:dw 0
a_noreloc:dw 0
dw 0
aout_end_v7:
aout_hdr:
head:dd 0
hdrlen:db 0
times 3 db 0
tsize:dd 0
dsize:dd 0
bsize:dd 0
entry:dd 0
total:dd 0
ssize:dd 0
trsize:dd 0
drsize:dd 0
tbase:dd 0
dbase:dd 0
aout_end:
exec:
a_midmag:
zerrm:db 'Negative size fault!',0
msg: db 'Parsing a.out...',0
e_hdr:
e_ident:times 0x10 db 0
e_type:dw 0
e_machine:dw 0
e_version: dd 0
e_entry:dd 0
e_phoff:dd 0
e_shoff:dd 0
e_flags:dd 0
e_ehsize:dw 0
e_phdrsize:dw 0
e_phnum:dw 0
e_shentsize:dw 0
e_shnum:dw 0
e_shstrndx: dw 0
e_end:
inc:dw 0
cntm:dw 0
memcpy:
mov word [cntm],0
pop bx
pop word cx
pop word si
pop word di
loopmcpy:
cmp word [cntm],cx
je rm
mov ax,[si]
mov byte [di],al
inc di
inc si
inc word [cntm]
jmp loopmcpy
rm:
jmp bx
phdr:
p_type:dd 0
p_offset:dd 0
p_vaddr:dd 0
p_paddr:dd 0
p_filesz:dd 0
p_memsz:dd 0
p_flags:dd 0
p_align:dd 0
phdr_end:
cntr:dd 0
_start:
mov bx,loading
call puts_bios
mov sp,0xffff
mov bp,sp
mov ax,ds
mov es,ax
mov ah,0x02
mov al,6
mov cl,4
mov ch,0
mov dh,0
mov bx,0x2000
mov dl,[0x50]
int 0x13
cmp ah,0
jne err
cmp byte [0x2000],1
jne err
cmp byte [0x2001],3
jne err
jmp aout
elf_start:
push e_hdr
push  0x2000
push e_end-e_hdr
call memcpy
mov bx,ldelf
call puts_bios
cmp byte [0x2000],0x7f
jne err
cmp byte [0x2001],'E'
jne err
cmp byte [0x2002],'L'
jne err
cmp byte [0x2003],'F'
jne err
mov word [cntr],0
loop2:
mov ax,[e_phnum]
cmp word [cntr],ax
je load_elf
xor ax,ax
mov al,[cntr]
mul word [e_phdrsize]
add ax,0x2000
add ax,[e_phoff]
push word phdr
push word ax
push word [e_phdrsize]
call memcpy
xor ax,ax
push word [p_vaddr]
mov ax,[p_offset]
add ax,0x2000
push ax
push word [p_memsz]
call memcpy
inc word [cntr]
jmp loop2
load_elf:
mov bx,jmpe
call puts_bios
mov di,sp
sploop:
mov byte [di],0
cmp di,0xffff
inc di
je jmp_elf_
jmp sploop
jmp_elf_:jmp [e_entry]
jmp err
err:mov bx,errmsg
call puts_bios
hng:jmp hng 
zerr:
mov bx,zerrm
call puts_bios
jmp hng
returned:mov bx,returnedmsg
call puts_bios
jmp hng
aout:
mov bx,msg
call puts_bios
push aout_hdr
push 0x2000
push aout_end-aout_hdr
call memcpy
push 0x7000
mov byte bx,[hdrlen]
add bx,0x2000
push bx
push word [tsize]
call memcpy
mov word bx,[tsize]
add bx,0x7000
push bx
mov bx,0x2000
add word bx,[tsize]
add byte bx,[hdrlen]
push bx
push word [dsize]
call memcpy
mov bx,jmpe
call puts_bios
jmp 0x7000
times 1024 - ($ - $$) db 0
