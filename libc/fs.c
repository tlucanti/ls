
#include <common.h>
#include <libc/fs.h>

DIR *do_opendir(const char *path)
{
	DIR *dir;

	dir = opendir(path);

	if (dir == NULL) {
		sys_error("opendir");
	}
	
	return dir;
}

struct dirent *do_readdir(DIR *dir)
{
	struct dirent *entry;

	errno = 0;

	entry = readdir(dir);
	BUG_ON(errno != 0, "corrupted DIR stream");

	return entry;
}

void do_closedir(DIR *dir)
{
	int ret = closedir(dir);

	if (ret != 0) {
		sys_error("closedir");
	}
}

int do_stat(const char *fname, struct stat *stat_entry)
{
	if (stat(fname, stat_entry)) {
		sys_error("stat");
		return EXIT_FAILURE;
	} else {
		return EXIT_SUCCESS;
	}
}

