
#ifndef _COMMON_H
# define _COMMON_H

# include <stdbool.h>
# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>

# ifndef __always_inline
#  define __always_inline inline __attribute__((always_inline))
# endif

# define container_of(ptr, type, member) ({                     \
    const typeof(((type *)0)->member) *__mptr = (void *)(ptr);  \
    (type*)((uintptr_t)__mptr - offsetof(type, member));        \
})

# define likely(expr) __builtin_expect(!!(expr), 1)
# define unlikely(expr) __builtin_expect(!!(expr), 0)

# define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

# define swap(lhs, rhs) do {                                    \
    (void)(&lhs == &rhs);                                       \
    typeof(lhs) c = lhs;                                        \
    lhs = rhs;                                                  \
    rhs = c;                                                    \
} while (false)

# define panic_on(expr, ...) do {                               \
    if (unlikely(expr)) {                                       \
        char *__args[] = {NULL, ##__VA_ARGS__};                 \
        if (ARRAY_SIZE(__args) > 1) {                           \
            printf("panic: %s:%d: %s\n", __FILE__, __LINE__,    \
                   (char *)__args[1]);                          \
        } else {                                                \
            printf("panic: %s:%d\n", __FILE__, __LINE__);       \
        }                                                       \
        abort();                                                \
    }                                                           \
} while (false)

#endif /* _COMMON_H */
