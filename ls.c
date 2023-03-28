
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <stddef.h>
#include <string.h>

void *xmalloc(size_t size)
{
    void *ptr;

    ptr = malloc(size);
    if (!ptr) {
        printf("malloc error\n");
        exit(1);
    }
    return ptr;
}

#define container_of(ptr, type, member) ({              \
                     const typeof( ((type*)0)->member ) \
                     * __mptr = ((void*)(ptr));         \
                     (type*)( (char*)__mptr -           \
                     offsetof(type, member) );          \
                     })

#define swap(lhs, rhs) do {                             \
    typeof(lhs) c = lhs;                                \
    lhs = rhs;                                          \
    rhs = c;                                            \
} while (false)

struct slist {
    struct slist *next;
};

struct file_list {
    struct slist slist_entry;
    struct dirent *file_entry;
    struct stat stat_entry;
};

struct dirent **ls(const char *path, size_t *file_cnt_ptr)
{
    size_t file_cnt = 0;
    DIR *dir;
    struct dirent *file_entry;
    struct file_list *list = NULL;
    struct file_list *next;
    struct dirent **file_array;

    dir = opendir(path);
    if (dir == NULL) {
        return NULL;
    }
    file_entry = readdir(dir);
    while (file_entry) {
        next = (struct file_list *)xmalloc(sizeof(struct file_list));
        next->file_entry = file_entry;
        next->slist_entry.next = &list->slist_entry;
        list = next;
        file_entry = readdir(dir);
        stat(file_entry->d_name, &);
        ++file_cnt;
    }
    file_array = (struct dirent **)xmalloc(sizeof(void *) * file_cnt);
    for (size_t i = 0; i < file_cnt; ++i) {
        file_array[i] = list->file_entry;
        next = container_of(list->slist_entry.next,
                            struct file_list,
                            slist_entry);
        free(list);
        list = next;
    }
    *file_cnt_ptr = file_cnt;
    return file_array;
}

typedef bool (*operator_t)(struct dirent *, struct dirent *);

void __qsort(struct dirent **arr,
             size_t size,
             operator_t operator,
             size_t left,
             size_t right)
{
    size_t li, ri, pi;

    li = left;
    ri = right;
    pi = (left + right) / 2;
    while (ri - li > 0) {
        while (li < pi && !operator(arr[li], arr[pi])) {
            li += 1;
        }
        while (ri > pi && operator(arr[ri], arr[pi])) {
            ri -= 1;
        }
        swap(arr[li], arr[ri]);
        if (pi == li) {
            pi = ri;
        } else if (pi == ri) {
            pi = li;
        }
    }
    if (pi - left > 1) {
        __qsort(arr, size, operator, left, pi);
    }
    if (right - pi > 1) {
        __qsort(arr, size, operator, pi, right);
    }
}

void ls_qsort(struct dirent **array, size_t size, operator_t operator)
{
    __qsort(array, size, operator, 0, size - 1);
}

bool acending_order(struct dirent *lhs, struct dirent *rhs)
{
    return strcmp(lhs->d_name, rhs->d_name) >= 0;
}

bool dot_filter(struct dirent *entry)
{
    return entry->d_name[0] != '.';
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    size_t size;
    struct dirent **file_array = ls(".", &size);
    ls_qsort(file_array, size, acending_order);
    for (size_t i = 0; i < size; ++i) {
        if (dot_filter(file_array[i])) {
            printf("%s\n", file_array[i]->d_name);
        }
    }
}
