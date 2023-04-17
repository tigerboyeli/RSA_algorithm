CC = gcc
CFLAGS = -Wall -g

all: rsa

rsa: main.o rsa.o
	$(CC) $(CFLAGS) main.o rsa.o -o rsa

main.o: main.c rsa.h
	$(CC) $(CFLAGS) -c main.c

rsa.o: rsa.c rsa.h
	$(CC) $(CFLAGS) -c rsa.c

clean:
	rm -f *.o rsa
