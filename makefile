CC = gcc
CFLAGS = -g 
MAIN = cache

.PHONY: all clean

all: main

$(MAIN): main.o 
	$(CC) $(CFLAGS) $^ -o $@ -no-pie

main.o: main.c
	$(CC) $(CFLAGS) -c cache.c

clean:
	rm -f *.o $(MAIN)