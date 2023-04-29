
all:
	gcc \
		-Wall -Wextra \
		-O0 -g3 -fno-omit-frame-pointer -fno-inline \
		-fdiagnostics-color=always -std=gnu2x \
		-I include \
		-D CONFIG_NO_LIBC \
		ls.c \
		-o ls
