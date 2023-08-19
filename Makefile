
all:
	clang \
		-Wall -Wextra \
		-O0 -g3 -fno-omit-frame-pointer -fno-inline \
		-fdiagnostics-color=always -std=c99 \
		-I include \
		libc/fs.c libc/io.c libc/malloc.c libc/string.c \
		src/common.c \
		ls2.c \
		-o ls
