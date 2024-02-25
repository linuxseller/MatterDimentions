CFLAGS=-Werror -Wextra -Wall -pedantic
CLIBS=-lncursesw
CC:=cc
all:
	$(CC) $(CFLAGS) src/main.c -o matter_dimentions $(CLIBS)
