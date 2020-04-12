CC=gcc
CFLAGS=-I.

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hexembed: hexembed.c
	$(CC) -o hexembed hexembed.c