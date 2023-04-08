
#ifndef _COMMON_H
# define _COMMON_H

# include <stdbool.h>
# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>

# ifndef __always_inline
#  define __always_inline inline __attribute__((always_inline))
# endif

# define ENOMEM (void *)0x1

# define container_of(ptr, type, member) ({                     \
    const typeof(((type *)0)->member) *__mptr = (void *)(ptr);  \
    (type*)((uintptr_t)__mptr - offsetof(type, member));        \
})

# define likely(expr) __builtin_expect(!!(expr), 1)
# define unlikely(expr) __builtin_expect(!!(expr), 0)

# define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
# define __number_to_string(number) #number
# define number_to_string(number) __number_to_string(number)
# define puts_literal(literal) write(STDOUT_FILENO, literal, sizeof(literal) - 1)

# define swap(lhs, rhs) do {                                    \
    (void)(&lhs == &rhs);                                       \
    typeof(lhs) c = lhs;                                        \
    lhs = rhs;                                                  \
    rhs = c;                                                    \
} while (false)

# define panic(msg) do {                                        \
    puts_literal("panic: " __FILE__ number_to_string(__LINE__)  \
                ": " msg "\n");                                 \
    exit(1);                                                    \
} while (false);

# define panic_on(expr, msg) do {                               \
    if (unlikely(expr)) {                                       \
        panic(msg);                                             \
    }                                                           \
} while (false)

static inline size_t strlen_impl(const char *string)
{
    size_t res = 0;

    while (*string) {
        ++string;
        ++res;
    }
    return res;
}

static inline void puts_impl(const char *string)
{
    write(STDOUT_FILENO, string, strlen_impl(string));
}

#endif /* _COMMON_H */
