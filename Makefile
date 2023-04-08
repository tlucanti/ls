
all:
	clang \
		-Wall -Wextra \
		-O0 -g3 -fno-omit-frame-pointer -fno-inline -ferror-limit=999 \
		-fdiagnostics-color=always -std=gnu2x \
		-I include \
		ls.c \
		-o ls
