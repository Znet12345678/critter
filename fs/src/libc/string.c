#include <string.h>
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
int strcat(char *dest,const char *src){
	return strcpy(&dest[strlen(dest)],src);
}

int strncmp(const char *str,const char *str2,unsigned int n){
	for(int i = 0; i < n;i++)
		if(*(str +i) != *(str2+i))
			return ++i;
	return 0;
}
int strcmp(const char *str,const char *str2){
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
void* memcpy(void *dest,const void *src,unsigned int n){
	for(int i = 0; i < n;i++)
		*((uint8_t*)dest + i) = *((uint8_t *)src + i);
	return dest;
}

