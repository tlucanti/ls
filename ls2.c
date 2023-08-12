
struct finfo {
	struct dirent dirent;
	struct stat stat;
};

struct finfo_list {
	struct finfo *array;
	size_t size;
}


void sys_error()
{
	const char *error_msg;
	int old_errno = errno;

	BUG_ON(errno == 0, "sys error with no errno");

	old_errno = errno;
	errno = 0;

	error_msg = strerror(old_errno);
	panic_on(errno != 0, "strerror error");

	puts_impl(error_msg);
}

void *do_malloc(size_t size)
{
	void *ret;

	ret = malloc(size);
	panic_on(ret == NULL, "no memory");

	return ret;
}

void *do_realloc(void *oldptr, size_t old_size, size_t new_size)
{
	void *new_ptr;

	new_ptr = do_malloc(new_size);
	memcpy_impl(new_ptr, old_ptr, old_size);

	return new_ptr;
}

DIR *do_opendir(const char *path)
{
	DIR *dir;

	dir = opendir(path);

	if (dir == NULL) {
		sys_error();
	}
	
	return dir;
}

struct dirent do_readdir(DIR *dir)
{
	struct dirent entry;

	errno = 0;

	entry = readdir(dir);
	BUG_ON(errno != 0, "corrupted DIR stream");

	return entry;
}

int do_stat(struct stat *stat_entry)
{
	if (stat(stat_entry)) {
		sys_error();
		return EXIT_FAILURE;
	} else {
		return EXIT_SUCCESS;
	}
}

int read_finfo(struct finfo *finfo, struct dirent *dirent)
{
	const char *fname = dirent->d_name;

	if (do_stat(&finfo->stat)) {
		return EXIT_FAILURE;
	}

	memcpy(&finfo->dirent, dirent, sizeof(struct dirent));

	return EXIT_SUCCESS;
}

int dir_walk(const char *path, struct finfo_list *list)
{
	const size_t dentry_init_size = 100;
	const size_t dentry_realloc_factor = 100;
	size_t dentry_alloc_size;

	DIR *dir;
	struct dirent *entry;

	list->array = NULL;
	list->size = 0;

	dir = do_opendir(path);
	if (dir == NULL) {
		return EXIT_FAILURE;
	}

	list->array = do_malloc(sizeof(struct dentry) * dentry_init_size);
	list->size = dentry_init_size;
	dentry_alloc_size = dentry_init_size;

	while (true) {
		entry = do_readdir(dir);
		if (entry == NULL) {
			break;
		}

		if (list->size == dentry_alloc_size) {
			dentry_alloc_size *= denry_realloc_factor;
			list->array = do_realloc(lst->array, list->size * sizeof(struct finfo), dentry_alloc_size * sizeof(struct finfo));
		}

		if (read_finfo(&list->array[list->size], entry) != 0) {
			continue;
		}
		++list->size;
	}

	return EXIT_SUCCESS;
}



int main() {

}
