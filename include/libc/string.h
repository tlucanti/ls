
#ifndef _LIBC_STRING_H
#define _LIBC_STRING_H

#include <stddef.h>

void memcpy_impl(void *restrict dst, const void *restrict src, size_t size);
int memcmp_impl(const void *restrict s1, const void *restrict s2, size_t size);
void *memdup_impl(void *restrict src, size_t size);
size_t strlen_impl(const char *restrict s);
int strcmp_impl(const char *restrict s1, const char *restrict s2);
bool streq_impl(const char *restrict s1, const char *restrict s2);
char *strdup_impl(const char *restrict s);

#endif /* _LIBC_STRING_H */
