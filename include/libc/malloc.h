
#ifndef _LIBC_MALLOC_H
#define _LIBC_MALLOC_H

#ifdef __linux__
#include <malloc.h>
#else 
#include <stdlib.h>
#endif

void *do_malloc(size_t size);
void *do_realloc(void *old_ptr, size_t old_size, size_t new_size);

#endif /* _LIBC_MALLOC_H */
