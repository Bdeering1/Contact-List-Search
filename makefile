CC = gcc
CFLAGS = -ansi -Wall -pedantic


all: hash0 hash1 hash2 hash3 parse

hash0: hash.c
	$(CC) $(CFLAGS) -D HASH0 hash.c -o $@

parse: hash.c
	$(CC) $(CFLAGS) -D PARSE hash.c -o $@

hash1: hash.c a4.o a4.h
	$(CC) $(CFLAGS) -D HASH1 hash.c a4.o -o $@

hash2: hash.c a4.o a4.h
	$(CC) $(CFLAGS) -D HASH2 hash.c a4.o -o $@

hash3: hash.c a4.o a4.h
	$(CC) $(CFLAGS) -D HASH3 hash.c a4.o -o $@


a4.o: a4.c a4.h
	$(CC) $(CFLAGS) -c a4.c -o $@


clean:
	rm hash0 hash1 hash2 hash3 a4.o