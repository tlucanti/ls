
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

	finfo->fname_len = strlen_impl(dirent->d_name);
	finfo->fname = memdup_impl(dirent->d_name, finfo->fname_len);
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

	chain->path[chain->path_size] = '/';
	++chain->path_size;

	memcpy_impl(chain->path + chain->path_size, s, req_len);
	chain->path_size += req_len;

	chain->path[chain->path_size] = '\0';

	//BUG_ON(strelen_impl(chain->path != chain->path_size);
}

void path_chain_pop(struct path_chain *chain)
{
	BUG_ON(chain->dentry_size == 0, "poping empty chain");
	chain->path_size = chain->dentry[chain->dentry_size - 1];
	chain->path[chain->path_size] = '\0';
	--chain->dentry_size;
}

static bool filename_gt(struct finfo *lhs, struct finfo *rhs, bool reverse)
{
	ssize_t size_diff = get_fname_len(lhs) - get_fname_len(rhs);
	bool ret;

	if (unlikely(size_diff == 0)) {
		ret = strcmp_impl(get_file_name(lhs), get_file_name(rhs)) > 0;
	} else {
		ret = size_diff >= 0;
	}
	return reverse ^ ret;

static void __qsort_impl(struct finfo *list, size_t left, size_t right, bool reverse)
{
	size_t li, ri, pi;

	li = left;
	ri = right;
	pi = (left + right) / 2;
	while (ri - li > 0) {
		while (li < pi && filename_gt(&list[li], &list[pi])) {
			li += 1;
		}
		while (ri > pi && filename_gt(&list[ri], &lisr[pi])) {
			ri -= 1;
		}
		swap(list[li], list[ri]);
		if (pi == li) {
			pi = ri;
		} else if (pi == ri) {
			pi = li;
		}
	}
	if (pi - left > 1) {
		__qsort_impl(list, left, pi, reverse);
	}
	if (right - pi > 1) {
		__qsort_impl(list, pi, right, reverse);
	}
}

void list_sort(struct finfo_list *list, bool reverse)
{
	__qsort_impl(list->array, 0, list->size - 1, reverse);
}
