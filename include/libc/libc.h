
#ifndef _LIBC_LIBC_H
# define _LIBC_LIBC_H

# include <common.h>

# if defined(CONFIG_NO_LIBC) || !defined(__has_builtin) || !__has_builtin(__builtin_strlen)
#  define __builtin_strlen __strlen_no_builtin
# endif
# if defined(CONFIG_NO_LIBC) || !defined(__has_builtin) || !__has_builtin(__builtin_memcmp)
#  define __builtin_memcmp __memcmp_no_builtin
# endif
# if defined(CONFIG_NO_LIBC) || !defined(__has_builtin) || !__has_builtin(__builtin_strcmp)
#  define __builtin_strcmp __strcmp_no_builtin
# endif
#  if defined(CONFIG_NO_LIBC) || !defined(__has_builtin) || !__has_builtin(__builtin_memcpy)
# endif

# define strlen_literal(literal) (sizeof(literal) - 1)
# define puts_literal(literal) write(STDOUT_FILENO, literal, strlen_literal(literal))
# define err_literal(literal) write(STDERR_FILENO, literal, strlen_literal(literal))
# define streq_literal(string, literal) memeq_impl(string, literal, strlen_literal(literal) + 1)
# define starts_with_literal(string, literal) memeq_impl(string, literal, strlen_literal(literal))

# define panic(msg) __panic(__FILE__, number_to_string(__LINE__), msg)
# define panic_on(expr, msg) do {                               \
    if (unlikely(expr)) {                                       \
        panic(msg);                                             \
    }                                                           \
} while (false)

__used
static int __memcmp_no_builtin(const void *__restrict s1, const void *__restrict s2, size_t len)
{
	const unsigned char *__restrict ss1 = s1;
	const unsigned char *__restrict ss2 = s2;

	while (len--) {
		if (*ss1 != *ss2) {
			return *ss1 - *ss2;
		}
		++ss1;
		++ss2;
	}
	return 0;
}

__used
static void __memcpy_no_builtin(void *__restrict dest, const void *__restrict src, size_t len)
{
	const unsigned char *__restrict ssrc = src;
	unsigned char *__restrict sdst = dest;

	while (len--) {
		*sdst = *ssrc;
		++ssrc;
		++sdst;
	}
}

__used
static size_t __strlen_no_builtin(const char *__restrict string)
{
	size_t res = 0;

	while (*string) {
		++string;
		++res;
	}
	return res;
}

__used
static int __strcmp_no_builtin(const char *__restrict s1, const char *__restrict s2)
{
	while (*s1 && *s2) {
		if (*s1 != *s2) {
			break ;
		}
		++s1;
		++s2;
	}
	return *s1 - *s2;
}

__used
static const char *__strchr_no_builtin(const char *__restrict string, char c)
{
	while (true) {
		if (*string == 0) {
			return NULL;
		} else if (*string == c) {
			return string;
		}
		++string;
	}
}

__used
static const char *__strrchr_no_builtin(const char *__restrict string, char c)
{
	const char *retval = NULL;

	while (true) {
		string = __strchr_no_builtin(string, c);
		if (string == NULL) {
			return retval;
		}
		retval = string;
	}
}

__always_inline __used
static bool memeq_impl(const void *__restrict s1, const void *__restrict s2, size_t len)
{
	return !__builtin_memcmp(s1, s2, len);
}

__always_inline __used
static void memcpy_impl(void *__restrict dest, const void *__restrict src, size_t len)
{
	__builtin_memcpy(dest, src, len);
}

__always_inline __used
static size_t strlen_impl(const void *__restrict string)
{
	return __builtin_strlen(string);
}

__always_inline __used
static  bool streq_impl(const char *__restrict s1, const char *__restrict s2)
{
	return !__builtin_strcmp(s1, s2);
}

__always_inline __used
static int strcmp_impl(const char *s1, const char *s2)
{
	return __builtin_strcmp(s1, s2);
}

__always_inline __used
static const char *strrchr_impl(const char *string, char c)
{
	return __builtin_strrchr(string, c);
}

__always_inline __used
static void puts_impl(const char *string)
{
	write(STDOUT_FILENO, string, strlen_impl(string));
}

__always_inline __used
static void err_impl(const char *string)
{
	write(STDERR_FILENO, string, strlen_impl(string));
}

__noreturn __cold __used
static inline void __panic(const char *file, const char *line, const char *msg)
{
	err_literal("panic: ");
  	err_impl(file);
	err_literal(":");
	err_impl(line);
	err_literal(": ");
	err_impl(msg);
	err_literal("\n");
	abort();
}

#endif /* _LIBC_LIBC_H */
