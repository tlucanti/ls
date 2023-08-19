
#ifndef _LS_LS_H
#define _LS_LS_H

struct finfo {
	char *fname;
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
