global _start
_start:
push ebp
mov byte [0xb8000],'!'
mov byte [0xb8001],15
hng:jmp hng
pop ebp
ret
