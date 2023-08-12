
#ifndef _LIBC_LIBC_H
#define _LIBC_LIBC_H

#include <common.h>

#if defined(CONFIG_NO_LIBC) || !defined(__has_builtin)
#define __use_builtin(__symbol) false
#else 
#define __use_builtin(__symbol) __has_builtin(__symbol)
#endif 

static inline void memcpy_impl(void *restrict dst, void *restrict src, size_t size)
{
#if __use_builtin(memcpy)
	return __builtin_memcpy(dst, src, size);
#endif

	while (size--) {
		*dst = *src;
		++dst;
		++src;
	}
}

#endif /* _LIBC_LIBC_H */
