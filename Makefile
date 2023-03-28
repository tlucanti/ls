
all:
	clang \
		-Wall -Wextra \
		-O0 -g3 -fno-omit-frame-pointer -fno-inline \
		-fdiagnostics-color=always -std=gnu2x \
		ls.c \
		-o ls
