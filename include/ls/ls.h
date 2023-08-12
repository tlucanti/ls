
#ifndef _LS_LS_H
#define _LS_LS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <common.h>
#include <libc/libc.h>
#include <libc/slist.h>

#define LS_INDICATOR_NONE 0x0
#define LS_INDICATOR_SLASH 0x1
#define LS_INDICATOR_CLASSIFY 0x2
#define LS_INDICATOR_FILE_TYPE 0x3

struct file_info {
	struct dirent *dir_entry;
	struct stat *stat_entry;
};

struct path_chain {
	struct slist slist_entry;
	size_t chain_string_end;
};

struct path_chain_head {
	char *merged;
	size_t allocated;
	size_t length;
	struct slist *slist_entry;
};

struct options {
	unsigned int show_hidden : 1;
	unsigned int show_dummy : 1;
	unsigned int ignore_backups : 1;
	unsigned int color : 1;
	unsigned int directory : 1;
	unsigned int indicator_type : 2;
	unsigned int show_owner : 1;
	unsigned int show_group : 1;
	unsigned int human_readable : 1;
	unsigned int inode : 1;
	unsigned int dereference : 1;
	unsigned int coma_separated : 1;
	unsigned int numeric_uid_gid : 1;
	unsigned int quote_name : 1;
	unsigned int reverse : 1;
	unsigned int recursive : 1;
	unsigned int show_size : 1;
	unsigned int u : 1;
	unsigned int dont_sort : 1;
	unsigned int c : 1;
	unsigned int table_print : 1;
	unsigned int one_per_line : 1;
	unsigned int multi_query : 1;
};

static __always_inline void path_chain_init(struct path_chain_head *head)
{
	head->merged = (char *)malloc(256);
	head->allocated = 256;
	head->length = 0;
	head->slist_entry = NULL;
}

static inline void path_chain_push(struct path_chain_head *path_chain,
				struct slist *slist_entry, const char *next_path)
{
	size_t next_path_length = strlen_impl(next_path);

	slist_entry->chain_string_end = path_chain->length;
	slist_push(path_chain->slist_entry, slist_entry);
	path_chain->merged[path_cahin->length++] = '/';
	if (unlikely(path_chain->length + next_path_length >= path_chain->allocated)) {
		path_chain->merged = realloc_impl(path_chain->merged, path_chain->length, path_chain->length * 2);
	}
	memcpy_impl(path_chain->merged + path_chain->length, next_path, next_path_length + 1);
	path_chain->length += next_path_length;
}

static inline void path_chain_pop(struct path_chain_head *path_chain)
{
	size_t chain_string_end;

	chain_string_end = slist_entry(path_chain->slist_entry, struct path_chain, slist_entry);
	path_chain->merged[chain_string_end] = '\0';
	path_chain->length = chain_string_end;
	slist_pop(path_chain);
}

#endif /* _LS_LS_H */
