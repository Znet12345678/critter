#include <stdio.h>
#include <stdlib.h>
char *gets(){
	char c;
	int i = 0;
	char *ret = malloc(1024);/*TODO MAKE DYNAMIC REALLOC*/
	while(i < 1024/*TMP*/){
		c = getc(STDIN);
		if(c == '\n')
			break;
		ret[i] = c;

		i++;
	}
	return ret;
}
