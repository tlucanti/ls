
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

struct file_info {
    struct dirent *dir_entry;
    struct stat *stat_entry;
};

struct path_chain {
    struct slist slist_entry;
    const char *name;
};

struct path_chain_head {
    struct path_chain begin;
    struct path_chain *end;
};

struct options {
    unsigned int all : 1;
    unsigned int recursive : 1;
    unsigned int reverse : 1;
};

struct argument_callbacks {
    bool (*compare)(struct file_info, struct file_info _rhs);
    void (*formatter)(struct file_info *file_info, struct options options);
};

#endif /* _LS_LS_H */
