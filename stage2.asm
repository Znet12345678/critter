[bits 16]
section .data
panic:db 'panic()',0
section .text
global _start
extern _puts
extern _load_stage3_1
_start:mov ah,0x0e
mov al,'*'
int 0x10
call _load_stage3_1
hng:jmp hng
global _panic
_panic:
push bp
mov bp,sp
push panic
call _puts
jmp hng
