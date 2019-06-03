void _start(){
	void **pntr = (void**)0x1000;
	int i = 0;
	while(pntr[i] != 0){
		i++;
	}
	int size = 0;
	i--;
	
	while(i >= 0){
		asm("push %0" : :"m"(pntr[i]));
		i--;
		size+=sizeof(pntr[i]);	
	}
	asm("call main\nadd %0,%%esp" : :"r"(size));	
}
