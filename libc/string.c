
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

int memcmp_impl(const void *restrict s1, const void *restrict s2, size_t size)
{
#if __use_builtin(__builtin_memcmp)
	return __builtin_memcmp(s1, s2, size);
#endif

	const unsigned char *restrict ss1 = s1;
	const unsigned char *restrict ss2 = s2;

	while (size--) {
		if (*ss1 != *ss2) {
			break;
		}
		++ss1;
		++ss2;
	}
	return *ss2 - *ss1;
}

void *memdup_impl(void *restrict src, size_t size)
{
	void *ret;

	ret = do_malloc(size);
	memcpy_impl(ret, src, size);
	return ret;
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

int strcmp_impl(const char *restrict s1, const char *restrict s2)
{
#if __use_builtin(__builtin_strcmp)
	return __builtin_strcmp(s1, s2);
#endif

	while (*s1 != *s2) {
		++s1;
		++s2;
	}
	return *s2 - *s1;
}

bool streq_impl(const char *restrict s1, const char *restrict s2)
{
	return strcmp_impl(s1, s2) == 0;
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

