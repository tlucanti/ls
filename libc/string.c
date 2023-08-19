
#include <common.h>
#include <libc/string.h>
#include <libc/malloc.h>

#if defined(__has_builtin) && !defined(CONFIG_NO_LIBC)
#define __use_builtin(sym) __has_builtin(sym)
#else 
#define __use_builtin false
#endif

void memcpy_impl(void *restrict dst, const void *restrict src, size_t size)
{
#if __use_builtin(__builtin_memcpy)
	__builtin_memcpy(dst, src, size);
	return;
#endif

	while (size--) {
		*(uint8_t *)dst = *(uint8_t *)src;
		++dst;
		++src;
	}
}

size_t strlen_impl(const char *restrict s)
{
#if __use_builtin(__builtin_strlen)
	return __builtin_strlen(s);
#endif

	size_t ret = 0;

	while (*s) {
		++ret;
		++s;
	}

	return ret;
}

char *strdup_impl(const char *restrict s)
{
#if __use_builtin(__builtin_strdup)
	return __builtin_strdup(s);
#endif

	size_t size;
	char *restrict ret;

	size = strlen_impl(s);
	ret = do_malloc(size * sizeof(char));
	memcpy_impl(ret, s, sizeof(char) * size);

	return ret;
}

