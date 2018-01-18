CC=gcc
CFLAGS=-g -Wall -lpthread -lm
DEPS = lab1_IO.c # put .h dependencies here

pth_BlockMatMult: pth_BlockMatMult.c $(DEPS)#lab1_IO.o
	$(CC) -o pth_BlockMatMult pth_BlockMatMult.c $(DEPS) $(CFLAGS)
