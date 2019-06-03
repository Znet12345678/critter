[bits 32]
section .data
oebp:dd 0
oesp:dd 0
section .text
global _start
extern main
_start:
mov dword [oebp],ebp
mov dword [oesp],esp
mov ebp,0xf00
times 2 push ebp
mov esp,ebp
mov edi,0x1000
mov al,0
loop:
cmp al,[0x999]
je endl
push dword [edi]
inc al
add edi,4
jmp loop
endl:
call main
mov ebp,[oebp]
mov esp,[oesp]
hng:jmp hng
ret
