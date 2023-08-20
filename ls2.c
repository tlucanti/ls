
#include <common.h>
#include <libc/libc.h>
#include <ls/ls.h>
#include <ls/util.h>

static int dir_walk(struct path_chain *path, struct finfo_list *list)
{
	const size_t array_init_size = 100;
	const size_t array_realloc_factor = 100;
	size_t array_alloc_size;
	int ret;

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

		path_chain_push(path, entry->d_name);
		ret = read_finfo(path->path, &list->array[list->size], entry);
		path_chain_pop(path);
		if (ret != 0) {
			continue;
		}
		++list->size;
	}

	do_closedir(dir);
	return EXIT_SUCCESS;
}

bool filter_fname(const char *fname, struct flags flags)
{
	if (fname[0] == '.') {
		return false;
	}
	return true;
}

static void ls(struct path_chain *chain, struct finfo_list *list, struct flags flags)
{
	const char *fname;

	dir_walk(chain, list);

	for (size_t i = 0; i < list->size; ++i) {
		fname = get_file_name(&list->array[i]);
		print_str(fname);
	}

	if (!flags.recursive) {
		return;
	}

	for (size_t i = 0; i < list->size; ++i) {
		if (!file_is_dir(&list->array[i])) {
			continue;
		}
		fname = get_file_name(&list->array[i]);
		if (streq_impl(fname, ".") || streq_impl(fname, "..")) {
			continue;
		}
		if (!filter_fname(fname, flags)) {
			continue;
		}
		path_chain_push(chain, fname);
		print_char('\n');
		print_str_raw(chain->path);
		print_str(":");
		ls(chain, list, flags);
		path_chain_pop(chain);
	}
}

int main() {
	struct finfo_list list;
	struct path_chain chain;
	struct flags flags = {
		.recursive = true
	};

	path_chain_init(&chain, "./");
	ls(&chain, &list, flags);
}
