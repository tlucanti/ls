
#include <common.h>
#include <libc/libc.h>
#include <ls/ls.h>
#include <ls/util.h>

int read_finfo(const char *path, struct finfo *finfo, struct dirent *dirent)
{
	struct stat stat_entry;

	if (do_stat(path, &stat_entry)) {
		return EXIT_FAILURE;
	}

	finfo->fname = strdup_impl(dirent->d_name);
	finfo->mode = stat_entry.st_mode;

	return EXIT_SUCCESS;
}

void clear_finfo(struct finfo *finfo)
{
	free(finfo->fname);
	free(finfo);
}

const char *get_file_name(struct finfo *finfo)
{
	return finfo->fname;
}

bool file_is_dir(struct finfo *finfo)
{
	return S_ISDIR(finfo->mode);
}

void path_chain_init(struct path_chain *chain, char *s)
{
	const size_t path_realloc_factor = 10;
	const size_t dentry_init_size = 10;

	size_t path_init_size = 1024;
	size_t len = strlen_impl(s);

	while (len > path_init_size) {
		path_init_size *= path_realloc_factor;
	}
	chain->path = do_malloc(path_init_size * sizeof(char));
	chain->dentry = do_malloc(dentry_init_size * sizeof(size_t));

	chain->path_alloc = path_init_size;
	chain->dentry_alloc = dentry_init_size;

	chain->path_size = len;
	chain->dentry_size = 0;

	memcpy_impl(chain->path, s, len + 1);
}

void path_chain_push(struct path_chain *chain, const char *s)
{
	const size_t chain_realloc_factor = 10;
	size_t req_len = strlen_impl(s);

	if (chain->dentry_size >= chain->dentry_alloc) {
		chain->dentry_alloc *= chain_realloc_factor;
		chain->dentry = do_realloc(
			chain->dentry, chain->dentry_size * sizeof(size_t),
			chain->dentry_alloc * sizeof(size_t));
	}
	chain->dentry[chain->dentry_size] = chain->path_size;
	++chain->dentry_size;

	if (chain->path_size + req_len + 2 >= chain->path_alloc) {
		do {
			chain->path_alloc *= chain_realloc_factor;
		} while (chain->path_size + req_len + 2 >= chain->path_alloc);
		chain->path = do_realloc(chain->path,
					 chain->path_size * sizeof(char),
					 chain->path_alloc * sizeof(char));
	}

	memcpy_impl(chain->path + chain->path_size, s, req_len);
	chain->path_size += req_len;

	chain->path[chain->path_size] = '/';
	chain->path[chain->path_size + 1] = '\0';
	++chain->path_size;

	memcpy_impl(chain->path + chain->path_size, s, req_len + 1);
}

void path_chain_pop(struct path_chain *chain)
{
	BUG_ON(chain->dentry_size == 0, "poping empty chain");
	chain->path_size = chain->dentry[chain->dentry_size - 1];
	chain->path[chain->path_size] = '\0';
	--chain->dentry_size;
}
