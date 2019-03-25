! 1 
! 1 # 1 "test.c"
! 1 # 3 "lib.h"
! 3 extern void putc();
!BCC_EOS
! 4 extern void puts();
!BCC_EOS
! 5 # 2 "test.c"
! 2 void test()
! 3 # 2 "test.c"
! 2 {
export	_test
_test:
! 3 	puts("Test\n");
push	bp
mov	bp,sp
push	di
push	si
! Debug: list * char = .1+0 (used reg = )
mov	bx,#.1
push	bx
! Debug: func () void = puts+0 (used reg = )
call	_puts
inc	sp
inc	sp
!BCC_EOS
! 4 	puts("Test2\n");
! Debug: list * char = .2+0 (used reg = )
mov	bx,#.2
push	bx
! Debug: func () void = puts+0 (used reg = )
call	_puts
inc	sp
inc	sp
!BCC_EOS
! 5 	hello();
! Debug: func () int = hello+0 (used reg = )
call	_hello
!BCC_EOS
! 6 }
pop	si
pop	di
pop	bp
ret
! 7 hello()
! Register BX used in function test
! 8 # 7 "test.c"
! 7 {
export	_hello
_hello:
! 8 	puts("\n\n\n\nHello, haruka!\n");
push	bp
mov	bp,sp
push	di
push	si
! Debug: list * char = .3+0 (used reg = )
mov	bx,#.3
push	bx
! Debug: func () void = puts+0 (used reg = )
call	_puts
inc	sp
inc	sp
!BCC_EOS
! 9 	return;
pop	si
pop	di
pop	bp
ret
!BCC_EOS
! 10 }
! 11 
! Register BX used in function hello
.data
.3:
.4:
.byte	$A,$A,$A,$A
.ascii	"Hello, haruka!"
.byte	$A
.byte	0
.2:
.5:
.ascii	"Test2"
.byte	$A
.byte	0
.1:
.6:
.ascii	"Test"
.byte	$A
.byte	0
.bss

! 0 errors detected
