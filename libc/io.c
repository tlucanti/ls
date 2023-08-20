
#include <common.h>
#include <libc/io.h>

#define PRINT_BUF_SIZE 1024
static char print_buf[PRINT_BUF_SIZE];
static unsigned int print_size = 0;

static void __do_print_flush(void)
{
	write(1, print_buf, print_size);
	print_size = 0;
}

void __print_flush(void)
{
	__do_print_flush();
}

void print_char(char c) 
{
	print_buf[print_size] = c;
	++print_size;

	if (c == '\n' || print_size == PRINT_BUF_SIZE) {
		__do_print_flush();
	}
}

void print_str_raw(const char *s)
{
	while (*s) {
		print_char(*s);
		++s;
	}
}

void print_str(const char *s)
{
	print_str_raw(s);
	print_char('\n');
}

static char __chr(unsigned long n)
{
	return (char)n + '0';
}

static void __print_uint(unsigned long num)
{
	char buf[20];
	int size = 0;

	while (num) {
		buf[size] = __chr(num % 10);
		num /= 10;
		++size;
	}

	while (size--) {
		print_char(buf[size]);
	}
}

void print_int(long num)
{
	if (unlikely(num == LONG_MIN)) {
		print_str(number_to_string(LONG_MIN));
	} else if (unlikely(num == 0)) {
		print_char('0');
	} else if (num < 0) {
		print_char('-');
		__print_uint(-num);
	} else {
		__print_uint(num);
	}
}

void print_uint(unsigned long num)
{
	if (unlikely(num == 0)) {
		print_char('0');
	} else {
		__print_uint(num);
	}
}

