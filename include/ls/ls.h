
#ifndef _LS_LS_H
#define _LS_LS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>

struct finfo {
	char *fname;
	size_t fname_len;
	mode_t mode;
};

struct finfo_list {
	struct finfo *array;
	size_t size;
};

struct path_chain {
	char *path;
	size_t *dentry;
	size_t path_size;
	size_t path_alloc;
	size_t dentry_size;
	size_t dentry_alloc;
};

struct flags {
	uint64_t recursive : 1;
};

#endif /* _LS_LS_H */
