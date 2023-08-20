
#ifndef _COMMON_H
#define _COMMON_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>

#ifndef __always_inline
#define __always_inline inline __attribute__((always_inline))
#endif
#ifndef __used
#define __used __attribute__((__used__))
#endif
#ifndef __unused
#define __unused __attribute__((__unused__))
#endif
#ifndef __cold
#define __cold __attribute__((__cold__))
#endif
#ifndef __noreturn
#define __noreturn __attribute__((__noreturn__))
#endif

#define container_of(ptr, type, member)                                    \
	({                                                                 \
		const typeof(((type *)0)->member) *__mptr = (void *)(ptr); \
		(type *)((uintptr_t)__mptr - offsetof(type, member));      \
	})

#if defined(__has_builtin) && __has_builtin(__builtin_expect)
#define likely(expr) __builtin_expect(!!(expr), 1)
#define unlikely(expr) __builtin_expect(!!(expr), 0)
#endif

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define __number_to_string(number) #number
#define number_to_string(number) __number_to_string(number)

#define swap(lhs, rhs)                \
	do {                          \
		(void)(&lhs == &rhs); \
		typeof(lhs) c = lhs;  \
		lhs = rhs;            \
		rhs = c;              \
	} while (false)

#define panic(reason) __panic(__FILE__, __LINE__, "panic", reason)
#define panic_on(expr, reason)         \
	do {                           \
		if (unlikely(expr)) {  \
			panic(reason); \
		}                      \
	} while (false)

#define BUG(reason) __panic(__FILE__, __LINE__, "BUG", reason)
#define BUG_ON(expr, reason)          \
	do {                          \
		if (unlikely(expr)) { \
			BUG(reason);  \
		}                     \
	} while (false)

void sys_error(const char *restrict source);

void __panic(const char *restrict file, unsigned long line,
	     const char *restrict source, const char *restrict reason);

#endif /* _COMMON_H */
