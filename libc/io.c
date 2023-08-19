
#include <common.h>
#include <libc/io.h>

#define PRINT_BUF_SIZE 1024
static char print_buf[PRINT_BUF_SIZE];
static unsigned int print_size = 0;

static inline void print_flush(void)
{
	write(1, print_buf, print_size);
	print_size = 0;
}

void print_char(char c) 
{
	if (c == '\n' || print_size == PRINT_BUF_SIZE) {
		print_flush();
	}

	print_buf[print_size] = c;
	++print_size;
}

void print_str(const char *s)
{
	while (*s) {
		print_char(*s);
		++s;
	}
}

void print_int(long num)
{
	if (unlikely(num == LONG_MIN)) {
		print_str(number_to_string(LONG_MIN));
		return;
	}

	print_char('-');
	print_uint(-num);
}

void print_uint(unsigned long num)
{
	char buf[20];
	int size = 0;

	if (unlikely(num == 0)) {
		print_char('0');
		return;
	}

	while (num) {
		buf[size] = num % 10;
		num /= 10;
	}

	for (int i = 0; i < size; ++i) {
		print_char(buf[i]);
	}
}

