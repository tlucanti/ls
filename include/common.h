
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
#ifndef __cold
#define __cold __attribute__((__cold__))
#endif
#ifndef __noreturn
#define __noreturn __attribute__((__noreturn__))
#endif

#define ENOMEM (void *)0x1

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

#endif /* _COMMON_H */
