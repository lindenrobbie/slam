CC = gcc
CFLAGS = -Wall -I./include

slam: src/main.c src/utils.c src/config.c src/wav_reader.c src/validator.c
		$(CC) $(CFLAGS) -o slam src/main.c src/utils.c src/config.c src/wav_reader.c src/validator.c

clean:
		rm -f slam