[bits 32]
[org 0xF000]
mov byte [0xb8000],'!'
mov byte [0xb8001],15
hng:jmp hng

