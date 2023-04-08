
#ifndef _LS_LS_H
# define _LS_LS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <dirent.h>
# include <stddef.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>

# include <common.h>

struct slist {
    struct slist *next;
};

struct file_list {
    struct slist slist_entry;
    struct dirent *file_entry;
    struct stat stat_entry;
};

struct options {
    unsigned int recursive : 1;
};

static inline void *xmalloc(size_t size)
{
    void *ptr;

    ptr = malloc(size);
    panic_on(ptr == NULL, "malloc error");
    return ptr;
}


#endif /* _LS_LS_H */
