
#ifndef _COMMON_H
#define _COMMON_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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

#define likely(expr) __builtin_expect(!!(expr), 1)
#define unlikely(expr) __builtin_expect(!!(expr), 0)

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

#define panic(msg) __panic(__FILE__, number_to_string(__LINE__), msg)
#define panic_on(expr, msg)           \
	do {                          \
		if (unlikely(expr)) { \
			panic(msg);   \
		}                     \
	} while (false)

__cold static inline __panic(const char *file, const char *line, const char *msg)
{
	puts_impl("PANIC: ");
	puts_impl(file);
	puts_impl(":");
	puts_impl(line);
	puts_impl("\nreason: ");
	puts_impl(msg);
	puts_impl("\n");
	exit(EXIT_FAILURE);
}

#endif /* _COMMON_H */
