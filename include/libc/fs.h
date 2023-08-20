
#ifndef _LIBC_FS_H
#define _LIBC_FS_H

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

DIR *do_opendir(const char *path);
struct dirent *do_readdir(DIR *dir);
void do_closedir(DIR *dir);
int do_stat(const char *fname, struct stat *stat_entry);

#endif /* _LIBC_FS_H */
