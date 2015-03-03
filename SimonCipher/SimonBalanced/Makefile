CC = gcc
CFLAGS = -g -Wall

all: simon

simon: simon.o simonMain.o
	$(CC) $(CFLAGS) -o simon simon.o simonMain.o

simonMain.o: simonMain.c simon.h
	$(CC) $(CFLAGS) -c simonMain.c

simon.o: simon.c simon.h
	$(CC) $(CFLAGS) -c simon.c


clean:
	rm -f simon *.o
	rm -f *~
