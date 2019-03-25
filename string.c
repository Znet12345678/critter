#include "lib.h"
char *strcpy(char *dest,const char *src){
	int i = 0;
	while(src[i] != 0){
		dest[i] = src[i];
		i++;
	}
	return dest;
}
unsigned int strlen(const char *str){
	int i = 0;
	while(str[i] != 0)
		i++;
	return i;
}
unsigned int strcat(char *dest,const char *src){
	strcpy(&dest[strlen(dest)],src);
}
#ifndef __PM
int strcmp(char *str,char *str2){
#else
int strcmp(const char *str,const char *str2){
#endif
	int i = 0;
	if(strlen(str) != strlen(str2))
		return -1;
	while(i < strlen(str)){
		if(str[i] != str2[i])
			return ++i;
		i++;
	}
	return 0;
}
#ifdef __PM
void* memcpy(void *dest,const void *src,unsigned int n){
	for(int i = 0; i < n;i++)
		*((uint8_t*)dest + i) = *((uint8_t *)src + i);
	return dest;
}
#endif
