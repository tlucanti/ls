
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
	size_t size;
	size_t alloc;
};

#endif /* _LS_LS_H */
