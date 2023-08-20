
#include <common.h>
#include <libc/io.h>
#include <string.h>

void sys_error(const char *source)
{
	const char *error_msg;
	int old_errno = errno;

	BUG_ON(errno == 0, "sys error with no errno");

	old_errno = errno;
	errno = 0;

	error_msg = strerror(old_errno);
	panic_on(errno != 0, "strerror error");

	__print_flush();
	print_str_raw("ls: ");
	print_str_raw(source);
	print_str_raw(": ");
	print_str_raw(error_msg);
	print_char('\n');
}

__cold void __panic(const char *restrict file, unsigned long line,
		    const char *restrict source, const char *restrict reason)
{
	__print_flush();
	print_str_raw(source);
	print_str_raw(": ");
	print_str_raw(file);
	print_char(':');
	print_uint(line);
	print_str_raw("\nreason: ");
	print_str_raw(reason);
	print_char('\n');
	exit(EXIT_FAILURE);
}

