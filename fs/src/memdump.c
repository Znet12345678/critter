#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int pow(int a,int b);
int atox(const char *str){
	int ret = 0;
	for(int i = strlen(str)-1; i >= 0;i--){
		if(str[i] >= '0' && str[i] <= '9'){
			ret+=pow(16,strlen(str)-i-1)*(str[i]-'0');
		}
		else if(str[i] >= 'a' && str[i] <= 'f')
			ret+=pow(16,strlen(str)-1-i)*(str[i]-'a'+10);
	}
	return ret;
}
pow(int a, int b){
	int ret = 1;
	for(int i = 0; i < b;i++)
		ret*=a;
	return ret;
}
int main(int argc,char *argv[]){

	struct Mem *pntr = (struct Mem *)0x00100000; 
	if(argc > 1)
		pntr = atox(argv[1]);
	while(1){
		puts("For address: 0x");
		putx(pntr);
		puts("\n");
		for(int i = 0; i < 23;i++){
			puts("0x");
			putx(pntr);
			puts(":");
			for(int j = 0; j < 0x10;j++){
				putx(*(uint8_t*)(pntr + j));
				puts(" ");
			}
			pntr+=2;
			puts("\n");
		}
		char c = getc(STDIN);
		if(c == 'q')
			break;
	}
}
