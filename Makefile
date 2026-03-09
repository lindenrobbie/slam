CC = gcc
CFLAGS = -Wall -I./include

slam: src/main.c src/utils.c src/config.c
		$(CC) $(CFLAGS) -o slam src/main.c src/utils.c src/config.c

clean:
		rm -f slam