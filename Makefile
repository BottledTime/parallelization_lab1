CC=gcc
CFLAGS=-g -Wall -lpthread
DEPS = lab1_IO.h timer.h # put .h dependencies here

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o #lab1_IO.o
	$(CC) -o main main.o $(CFLAGS)
