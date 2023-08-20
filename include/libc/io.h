
#ifndef _LIBC_IO_H
#define _LIBC_IO_H

#include <unistd.h>

void print_char(char c);
void print_str_raw(const char *s);
void print_str(const char *s);
void print_int(long num);
void print_uint(unsigned long num);

void __print_flush(void);

#endif /* _LIBC_IO_H */
