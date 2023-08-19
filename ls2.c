
#include <common.h>
#include <libc/libc.h>

#include <ls/ls.h>

#include <libc/string.h>

static const char *get_file_name(struct finfo *finfo)
{
	return finfo->fname;
}

static int read_finfo(const char *path, struct finfo *finfo, struct dirent *dirent)
{
	struct stat stat_entry;

	if (do_stat(path, &stat_entry)) {
		return EXIT_FAILURE;
	}

	finfo->fname = strdup_impl(dirent->d_name);

	return EXIT_SUCCESS;
}

static void clear_finfo(struct finfo *finfo)
{
	free(finfo->fname);
	free(finfo);
}

static int dir_walk(struct path_chain *path, struct finfo_list *list)
{
	const size_t array_init_size = 100;
	const size_t array_realloc_factor = 100;
	size_t array_alloc_size;

	DIR *dir;
	struct dirent *entry;

	list->array = NULL;
	list->size = 0;

	dir = do_opendir(path->path);
	if (dir == NULL) {
		return EXIT_FAILURE;
	}

	list->array = do_malloc(sizeof(struct finfo) * array_init_size);
	list->size = 0;
	array_alloc_size = array_init_size;

	while (true) {
		entry = do_readdir(dir);
		if (entry == NULL) {
			break;
		}

		if (list->size == array_alloc_size) {
			array_alloc_size *= array_realloc_factor;
			list->array = do_realloc(list->array, list->size * sizeof(struct finfo), array_alloc_size * sizeof(struct finfo));
		}

		if (read_finfo(path->path, &list->array[list->size], entry) != 0) {
			continue;
		}
		++list->size;
	}

	return EXIT_SUCCESS;
}

static void path_chain_push(struct path_chain *chain, const char *s)
{
	const size_t chain_realloc_factor = 10;
	size_t req_len = strlen_impl(s);

	if (chain->dentry_size >= chain->dentry_alloc) {
		chain->dentry_alloc *= chain_realloc_factor;
		chain->dentry = do_realloc(chain->dentry, chain->dentry_size * sizeof(size_t), chain->dentry_alloc * sizeof(size_t));
	}
	chain->dentry[chain->dentry_size] = chain->path_size;

	if (chain->path_size + req_len + 2 >= chain->path_alloc) {
		chain->path_alloc *= chain_realloc_factor;
		chain->path = do_realloc(chain->path, chain->path_size * sizeof(char), chain->path_alloc * sizeof(char));
	}
	chain->path[chain->path_size] = '/';
	++chain->path_size;

	memcpy_impl(chain->path + chain->path_size, s, req_len + 1);
}

static void path_chain_pop(struct path_chain *chain)
{
	chain->path_size = chain->dentry[chain->dentry_size - 1];
	chain->path[chain->path_size] = '\0';
	--chain->dentry_size;
}

static void ls(struct path_chain *chain, struct finfo_list *list, struct flags flags)
{
	const char *fname;

	dir_walk(chain, list);

	for (size_t i = 0; i < list->size; ++i) {
		fname = get_file_name(&list->array[i]);
		print_str(fname);
		print_char('\n');
	}

	if (!flags.recursive) {
		return;
	}

	for (size_t i = 0; i < list->size; ++i) {
		fname = get_file_name(&list->array[i]);
		print_str_raw(chain->path);
		print_str(":");
		path_chain_push(chain, fname);
		ls(chain, list, flags);
		path_chain_pop(chain);
	}
}

int main() {
	struct flags flags = {
		.recursive = true
	};
	struct finfo_list list;
	struct path_chain chain = (struct path_chain){
		.path = strdup_impl(".")
	};

	ls(&chain, &list, flags);
}
