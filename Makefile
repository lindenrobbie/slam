CC = gcc
CFLAGS = -Wall -I./include

slam: src/main.c src/utils.c
		$(CC) $(CFLAGS) -o slam src/main.c src/utils.c

clean:
		rm -f slam