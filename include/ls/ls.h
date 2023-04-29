
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
# include <libc/libc.h>

# define LS_INDICATOR_NONE      0x0
# define LS_INDICATOR_SLASH     0x1
# define LS_INDICATOR_CLASSIFY  0x2
# define LS_INDICATOR_FILE_TYPE 0x3

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
    struct slist *end;
};

struct options {
    unsigned int show_hidden : 1;
    unsigned int show_dummy : 1;
    unsigned int ignore_backups : 1;
    unsigned int color : 1;
    unsigned int directory : 1;
    unsigned int indicator_type : 2;
    unsigned int show_owner : 1;
    unsigned int show_group : 1;
    unsigned int human_readable : 1;
    unsigned int inode : 1;
    unsigned int dereference : 1;
    unsigned int coma_separated : 1;
    unsigned int numeric_uid_gid : 1;
    unsigned int quote_name : 1;
    unsigned int reverse : 1;
    unsigned int recursive : 1;
    unsigned int show_size : 1;
    unsigned int u : 1;
    unsigned int dont_sort : 1;
    unsigned int c : 1;
    unsigned int table_print : 1;
    unsigned int one_per_line : 1;
    unsigned int multi_query : 1;
};

#endif /* _LS_LS_H */
