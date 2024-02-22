CFLAGS=-Werror -Wextra -Wall -pedantic
CLIBS=-lncurses
CC:=cc
all:
	$(CC) $(CFLAGS) $(CLIBS) src/main.c -o matter_dimentions
