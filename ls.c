
#include <ls/ls.h>

static size_t __get_dir_size(const char *path)
{
    size_t size = 0;
    DIR *dir;

    dir = opendir(path);
    panic_on(dir == NULL, "readdir error");
    while (readdir(dir)) {
        ++size;
    }
    closedir(dir);
    return size;
}

struct file_info *get_entries(const char *path, size_t *file_cnt_ptr)
{
    size_t file_cnt;
    DIR *dir;
    struct dirent *dir_entry;
    struct file_info *file_array;

    file_cnt = __get_dir_size(path);
    dir = opendir(path);
    panic_on(dir == NULL, "opendir error");
    file_array = (struct file_info *)malloc(sizeof(struct file_info) * file_cnt);
    if (file_array == NULL) {
        return ENOMEM;
    }
    for (size_t i = 0; i < file_cnt; ++i) {
        dir_entry = readdir(dir);
        if (unlikely(dir_entry == NULL)) {
            file_cnt = i;
            break ;
        }
        file_array[i].dir_entry = dir_entry;
        file_array[i].stat_entry = (struct stat *)malloc(sizeof(struct stat));
        if (file_array[i].stat_entry == NULL) {
            return ENOMEM;
        }
        stat(file_array[i].dir_entry->d_name, file_array[i].stat_entry);
    }
    closedir(dir);
    *file_cnt_ptr = file_cnt;
    return file_array;
}

static inline void print_file(struct file_info file_info, struct options options)
{
    if (file_info.dir_entry->d_name[0] == '.' && !options.all) {
        return ;
    }
    puts_impl(file_info.dir_entry->d_name);
    puts_literal("\n");
}

static void __print_path_chain(struct path_chain *chain)
{
    while (chain) {
        puts_impl(chain->name);
        chain = container_of(chain->slist_entry.next,
                             struct path_chain,
                             slist_entry);
    }
}

static void __qsort_impl(struct file_info *data,
             bool (*compare)(struct file_info lhs, struct file_info rhs),
             size_t left,
             size_t right,
             bool reverse)
{
    (void)reverse;
    size_t li, ri, pi;

    li = left;
    ri = right;
    pi = (left + right) / 2;
    while (ri - li > 0) {
        while (li < pi && !compare(data[li], data[pi])) {
            li += 1;
        }
        while (ri > pi && compare(data[ri], data[pi])) {
            ri -= 1;
        }
        swap(data[li], data[ri]);
        if (pi == li) {
            pi = ri;
        } else if (pi == ri) {
            pi = li;
        }
    }
    if (pi - left > 1) {
        __qsort_impl(data, compare, left, pi, reverse);
    }
    if (right - pi > 1) {
        __qsort_impl(data, compare, pi, right, reverse);
    }
}

void qsort_impl(struct file_info *data,
              size_t size,
              bool (*compare)(struct file_info lhs, struct file_info rhs),
              bool reverse)
{
    __qsort_impl(data, compare, 0, size - 1, reverse);
}

void ls(const char *path,
        struct argument_callbacks *callbacks,
        struct options options,
        struct path_chain_head *path_chain)
{
    struct file_info *file_array;
    size_t size;
    struct path_chain chain_entry;

    file_array = get_entries(path, &size);
    if (unlikely(file_array == ENOMEM)) {
        panic("no memory");
    }
    qsort_impl(file_array, size, callbacks->compare, options.reverse);
    for (size_t i = 0; i < size; ++i) {
        print_file(file_array[i], options);
    }
    if (options.recursive) {
        for (size_t i = 0; i < size; ++i) {
            __print_path_chain(&path_chain->begin);
            chain_entry.name = file_array[i].dir_entry->d_name;
            path_chain->end->slist_entry.next = &chain_entry.slist_entry;
            path_chain->end = &chain_entry;
            if (S_ISDIR(file_array[i].stat_entry->st_mode)) {
                ls(file_array[i].dir_entry->d_name, callbacks, options, path_chain);
            }
        }
    }
}

static inline bool sort_by_name(struct file_info lhs, struct file_info rhs)
{
    return strcmp(lhs.dir_entry->d_name, rhs.dir_entry->d_name) >= 0;
}

static inline bool sort_by_size(struct file_info lhs, struct file_info rhs)
{

}

static inline bool sort_by_time(struct file_info lhs, struct file_info rhs)
{

}

static inline bool sort_by_version(struct file_info lhs, struct file_info rhs)
{

}

static inline bool sort_by_extension(struct file_info lhs, struct file_info rhs)
{

}

void argparse(char **argv,
              bool (**sort_function)(struct file_info lhs, struct file_info rhs),
                     struct options *options)
{
    char *binary_name = *argv;
    char *s;

    *sort_function = sort_by_name
    callbacks->compare = ascending_order;
    options->show_hidden = 0;
    options->show_dummy = 0;
    options->ignore_backups = 0;
    options->color = isatty(STDOUT_FILENO);
    options->directory = 0;
    options->indicator_type = LS_INDICATOR_NONE;
    options->show_owner = 1;
    options->show_group = 1;
    options->human_readable = 0;
    options->inode = 0;
    options->dereference = 0;
    options->coma_separated = 0;
    options->numeric_uid_gid = 0;
    options->quote_name = 0;
    options->reverse = 1;
    options->recursive = 1;
    optione->show_size = 0;
    options->u = 0;
    while (*argv) {
        if (*argv[0] == '-') {
            if (*argv[1] == '-') {
                if (streq_literal(*argv, "--all")) {
                    options->show_hidden = 1;
                    options->show_dummy = 1;
                } else if (streq_literal(*argv, "--almost-all")) {
                    options->show_hidden = 1;
                    options->show_dummy = 0;
                } else if (streq_literal(*argv, "--ignore-backups")) {
                    options->ignore_backups = 1;
                } else if (streq_literal(*argv, "--directory")) {
                    options->directory = 1;
                } else if (streq_literal(*argv, "--color")) {
                    options->color = 1;
                } else if (starts_with_literal(*argv, "--color=")) {
                    if (streq_literal(*argv + 8, "always") ||
                        streq_literal(*argv + 8, "yes") ||
                        streq_literal(*argv + 8, "force")) {
                        options->color = 1;
                    } else if (streq_literal(*argv + 8, "auto") ||
                               streq_literal(*argv + 8, "tty") ||
                               streq_literal(*argv + 8, "if-tty")) {
                        options->color = isatty(STDOUT_FILENO);
                    } else if (streq_literal(*argv + 8, "never") ||
                               streq_literal(*argv + 8, "no") ||
                               streq_literal(*argv + 8, "none")) {
                        options->color = 0;
                    } else {
                        puts_impl(binary_name);
                        puts_literal(": invalid argumet `");
                        puts_impl(*argv + 8);
                        puts_literal("` for `--color`\n"
                                     "Valid arguments are:\n"
                                     "  - `always`, `yes`, `force`\n"
                                     "  - `never`, `no`, `none`\n"
                                     "  - `auto`, `tty`, `if-tty`\n"
                                     "Try `ls --help` for more information.\n");
                        exit(1);
                    }
                } else if (streq_literal(*argv, "--classify")) {
                    options->indicator_type = LS_INDICATOR_CLASSIFY;
                } else if (streq_literal(*argv, "--file-type")) {
                    options->indicator_type = LS_INDICATOR_FILE_TYPE;
                } else if (streq_literal(*argv, "--no-group")) {
                    options->show_group = 0;
                } else if (streq_literal(*argv, "--human-readable")) {
                    options->human_readable = 1;
                } else if (streq_literal(*argv, "--inode")) {
                    options->inode = 1;
                } else if (streq_literal(*argv, "--dereference")) {
                    options->dereference = 1;
                } else if (streq_literal(*argv, "--numeric-uid-gid")) {
                    options->numeric_uid_gid = 1;
                } else if (streq_literal(*argv, "--indicator-style")) {
                    puts_impl(binary_name);
                    puts_literal(": options `--indicator-style` requires an argument\n"
                                 "Try `ls --help` for more information\n");
                    exit(1);
                } else if (starts_with_literal(*argv, "--indicator-style=")) {
                    if (streq_literal(*argv + 18, "none")) {
                        options->indicator_type = LS_INDICATOR_NONE;
                    } else if (streq_literal(*argv + 18, "slash")) {
                        options->indicator_type = LS_INDICATOR_SLASH;
                    } else if (streq_literal(*argv + 18, "file-type")) {
                        options->indicator_type = LS_INDICATOR_FILE_TYPE;
                    } else if (streq_literal(*argv + 18, "classify")) {
                        options->indicator_type = LS_INDICATOR_CLASSIFY;
                    } else {
                        puts_impl(binary_name);
                        puts_literal(": invalid argument `");
                        puts_impl(*argv + 18);
                        puts_literal("` for `--indicator-style`\n"
                                     "Valid arguments are:\n"
                                     "  - `none`\n"
                                     "  - `slash`\n"
                                     "  - `file-type`\n"
                                     "  - `classify`\n"
                                     "Try `ls --help` for more information.\n");
                        exit(1);
                    }
                } else if (streq_literal(*argv, "--quote-name")) {
                    options->quote_name = 1;
                } else if (streq_literal(*argv, "--reverse")) {
                    options->reverse = 1;
                } else if (streq_literal(*argv, "--recursive")) {
                    options->recursive = 1;
                } else if (streq_literal(*argv, "--size")) {
                    options->show_size = 1;
                } else if (streq_literal(*argv, "--sort")) {
                    puts_impl(binary_name);
                    puts_literal(": options `--sort` requires an argument\n"
                                 "Try `ls --help` for more information\n");
                    exit(1);
                } else if (starts_with_literal(*argv, "--sort=")) {
                    if (streq_literal(*argv + 7, "none")) {
                        options->dont_sort = 1;
                    } else if (streq_literal(*argv + 7, "size")) {
                        options->dont_sort = 0;
                        *sort_function = sort_by_size;
                    } else if (streq_literal(*argv + 7, "time")) {
                        options->dont_sort = 0;
                        *sort_function = sort_by_time;
                    } else if (streq_literal(*argv + 7, "version")) {
                        options->dont_sort = 0;
                        *sort_function = sort_by_version;
                    } else if (streq_literal(*argv + 7, "extension")) {
                        options->dont_sort = 0;
                        *sort_function = sort_by_extension;
                    } else {
                        puts_impl(binary_name);
                        puts_literal(": invalid argument `");
                        puts_impl(*argv + 18);
                        puts_literal("` for `--indicator-style`\n"
                                     "Valid arguments are:\n"
                                     "  - `none`\n"
                                     "  - `time`\n"
                                     "  - `size`\n"
                                     "  - `extension`\n"
                                     "  - `version`\n"
                                     "Try `ls --help` for more information.\n");
                        exit(1);
                    }
                } else if (streq_literal(*argv, "--")) {
                    /* nothing */
                } else {
                    puts()
                    exit(1);
                }
            } else {
                s = *argv + 1;
                while (*s) {
                    switch (*s) {
                    case 'a':
                        options->show_hidden = 1;
                        options->show_dummy = 1;
                        break ;
                    case 'A':
                        options->show_hidden = 1;
                        options->show_dummy = 0;
                        break ;
                    case 'B':
                        options->ignore_backups = 1;
                        break ;
                    case 'c':
                        options->c = 1;
                        break ;
                    case 'd'
                        options->directory = 1;
                        break ;
                    case 'f':
                        options->dont_sort = 1;
                        options->show_hidden = 1;
                        options->show_dummy = 1;
                        options->long_format = 0;
                        options->show_size = 0;
                        options->color = 0;
                        break ;
                    case 'F':
                        options->classify = 1;
                        options->file_type = 1;
                        break ;
                    case 'g':
                        options->show_owner = 0;
                        options->long_format = 1;
                        break ;
                    case 'G':
                        options->show_group = 0;
                        break ;
                    case 'h':
                        options->human_readable = 1;
                        break ;
                    case 'i':
                        options->inodes = 1;
                        break ;
                    case 'k':
                        break ;
                    case 'l':
                        options->long_format = 1;
                        break ;
                    case 'L':
                        options->dereference = 1;
                        break ;
                    case 'm':
                        options->coma_separated = 1;
                        break ;
                    case 'n':
                        options->numeric_uid_gid = 1;
                        break ;
                    case 'o':
                        options->long_format = 1;
                        options->show_group = 0;
                        break ;
                    case 'p':
                        options->indicator_type = LS_INDICATOR_SLASH;
                        break ;
                    case 'Q':
                        options->quote_name = 1;
                        break ;
                    case 'r':
                        options->reverse = 1;
                        break ;
                    case 'R':
                        options->recursive = 1;
                        break ;
                    case 's':
                        options->show_size = 1;
                        break ;
                    case 'S':
                        options->dont_sort = 0;
                        *sort_function = sort_by_size;
                        break ;
                    case 't':
                        options->dont_sort = 0;
                        *sort_function = sort_by_time;
                        break ;
                    case 'u':
                        options->u = 1;
                        break ;
                    case 'U':
                        options->dont_sort = 1;
                        break ;
                    case 'v':
                        options->dont_sort = 0;
                        *sort_function = sort_by_version;
                        break ;
                    case 'x':
                        options->
                        break ;
                    case 'X':
                        options->dont_sort = 0;
                        *sort_function = sort_by_extension;
                        break ;
                    case '1':
                        options->one_per_line = 1;
                        break ;
                    default:
                        puts_impl(binary_name);
                        puts_literal(": invalid option -- `");
                        write(STDOUT_FILENO, s, 1);
                        puts_impl(*argv);
                        puts_literal("Try `ls --help` for more information.\n");
                        exit(1);
                    }
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    struct argument_callbacks callbacks;
    struct options options;
    struct path_chain_head head;
    const char *begin_path;

    begin_path = argparse(argv, &callbacks, &options);
    head.end = &head.begin;

    ls(begin_path, &callbacks, options, &head);
}
