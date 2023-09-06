
#ifndef _LS_UTIL_H
#define _LS_UTIL_H

int read_finfo(const char *path, struct finfo *finfo, struct dirent *dirent);
void clear_finfo(struct finfo *finfo);
const char *get_file_name(struct finfo *finfo);
bool file_is_dir(struct finfo *finfo);

void path_chain_init(struct path_chain *chain, char *s);
void path_chain_push(struct path_chain *chain, const char *s);
void path_chain_pop(struct path_chain *chain);

void list_sort(struct finfo_list *list, bool reverse);

#endif /* _LS_UTIL_H */
