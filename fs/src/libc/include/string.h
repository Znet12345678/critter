
#ifndef __STRING_H
#define __STRING_H
#include <stdint.h>
int strcmp(const char *one,const char *two);
int strncmp(const char *one,const char *two,unsigned int n);
void *memcpy(void *dest,const void *src,unsigned int n);
char *strcpy(char *dest,const char *src);
unsigned int strlen(const char *str);
char **sep(const char *str,char c);
int strcat(char *dest,const char *src);
#endif
