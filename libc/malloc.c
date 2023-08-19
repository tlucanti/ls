
#include <common.h>
#include <libc/malloc.h>
#include <libc/string.h>

void *do_malloc(size_t size)
{
	void *ret;

	ret = malloc(size);
	panic_on(ret == NULL, "no memory");

	return ret;
}

void *do_realloc(void *old_ptr, size_t old_size, size_t new_size)
{
	void *new_ptr;

	new_ptr = do_malloc(new_size);
	memcpy_impl(new_ptr, old_ptr, old_size);

	return new_ptr;
}

