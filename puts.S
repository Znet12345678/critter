! 1 
! 1 # 1 "puts.c"
! 1 # 3 "lib.h"
! 3 extern void putc();
!BCC_EOS
! 4 extern void puts();
!BCC_EOS
! 5 # 2 "puts.c"
! 2 void puts(str)
! 3 # 2 "puts.c"
! 2 char *str;
export	_puts
_puts:
!BCC_EOS
! 3 # 2 "puts.c"
! 2 {
! 3 	int i = 0;
push	bp
mov	bp,sp
push	di
push	si
dec	sp
dec	sp
! Debug: eq int = const 0 to int i = [S+8-8] (used reg = )
xor	ax,ax
mov	-6[bp],ax
!BCC_EOS
! 4 	while(str[i] != 0){
jmp .2
.3:
! 5 		putc(str[i]);	
! Debug: ptradd int i = [S+8-8] to * char str = [S+8+2] (used reg = )
mov	ax,-6[bp]
add	ax,4[bp]
mov	bx,ax
! Debug: list char = [bx+0] (used reg = )
mov	al,[bx]
xor	ah,ah
push	ax
! Debug: func () void = putc+0 (used reg = )
call	_putc
inc	sp
inc	sp
!BCC_EOS
! 6 		i++;
! Debug: postinc int i = [S+8-8] (used reg = )
mov	ax,-6[bp]
inc	ax
mov	-6[bp],ax
!BCC_EOS
! 7 	}
! 8 	return;
.2:
! Debug: ptradd int i = [S+8-8] to * char str = [S+8+2] (used reg = )
mov	ax,-6[bp]
add	ax,4[bp]
mov	bx,ax
! Debug: ne int = const 0 to char = [bx+0] (used reg = )
mov	al,[bx]
test	al,al
jne	.3
.4:
.1:
inc	sp
inc	sp
pop	si
pop	di
pop	bp
ret
!BCC_EOS
! 9 }
! 10 
! Register BX used in function puts
.data
.bss

! 0 errors detected
