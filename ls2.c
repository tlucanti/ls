
#include <common.h>
#include <libc/libc.h>

#include <ls/ls.h>

#include <libc/string.h>

const char *get_file_name(struct finfo *finfo)
{
	return finfo->fname;
}

int read_finfo(const char *path, struct finfo *finfo, struct dirent *dirent)
{
	struct stat stat_entry;

	if (do_stat(path, &stat_entry)) {
		return EXIT_FAILURE;
	}

	finfo->fname = strdup_impl(dirent->d_name);

	return EXIT_SUCCESS;
}

void clear_finfo(struct finfo *finfo)
{
	free(finfo->fname);
	free(finfo);
}

int dir_walk(struct path_chain *path, struct finfo_list *list)
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

int main() {
	struct finfo_list list;
	struct path_chain chain = (struct path_chain){
		.path = strdup_impl(".")
	  };

	dir_walk(&chain, &list);
	
	for (size_t i = 0; i < list.size; ++i) {
		print_str(get_file_name(&list.array[i]));
		print_char('\n');
	}
}
