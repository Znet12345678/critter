[bits 32]
global _start
extern load_stage_3
_start:push ebp
mov ebp,esp
jmp load_stage_3
pop ebp
ret
