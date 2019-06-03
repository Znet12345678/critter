#ifndef __STDIO_H
#define __STDIO_H
#define STDIN -2
void puts(const char *str);
char getc(int fd);
void putc(char c);
void putx(int c);
char *gets();
#endif
