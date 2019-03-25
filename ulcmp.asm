global _ulcmp
_ulcmp:
push bp
mov eax,[bp+4]
mov ebx,[bp+6]
cmp eax,ebx
je one
mov ax,0
ret
one:
mov ax,1
ret
