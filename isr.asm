[bits 32]
section .isr
extern free
extern puts
extern open
extern llread
extern malloc
extern exec
extern putc
extern getc
extern putx
extern sep
cld
push ebp
mov ebp,esp
cmp ah,0x1
je _puts
cmp ah,0x2
je _open
cmp ah,0x3
je _read
cmp ah,0x4
je _malloc
cmp ah,0x5
je _exec
cmp ah,0x6
je _getc
cmp ah,0x7
je _free
cmp ah,0x8
je _putc
cmp ah,0x9
je _putx
cmp ah,0x0a
je _sep
iret
_putc:
push ebx
call putc
add esp,4
pop ebp
iret
_puts:
push ebx
call puts
add esp,4
pop ebp
iret
_open:
push ecx
call open
add esp,4
pop ebp
iret
_read:
push edx
push ecx
push ebx
call llread
add esp,12
pop ebp
iret
_malloc:
push ebx
call malloc
add esp,4
pop ebp
iret
_exec:
push edx
push ecx
push ebx
call exec
add esp,12
pop ebp
iret
_getc:
push ebx
call getc
add esp,4
pop ebp
iret
_free:
push ebx
call free
add esp,4
pop ebp
iret
_putx:
push ebx
call putx
add esp,4
pop ebp
iret
_sep:
push ecx
push ebx
call sep
add esp,8
pop ebp
iret
