
#ifndef _LIBC_STRING_H
#define _LIBC_STRING_H

#include <stddef.h>

void memcpy_impl(void *restrict dst, const void *restrict src, size_t size);
size_t strlen_impl(const char *restrict s);
char *strdup_impl(const char *restrict s);

#endif /* _LIBC_STRING_H */
