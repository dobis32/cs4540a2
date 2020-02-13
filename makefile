#
#	AUTHOR: SCOTT VANDERWEIDE
#	CS 4540
#	ASSIGNMENT 1
#
CC=gcc
CFLAGS= -ggdb -Wall -pedantic -std=c11

make: clean readProcesses.o cpu.o processActions.o io.o queue.o
	$(CC) $(CFLAGS) -o a2 main.c

readProcesses.o:
	$(CC) $(CFLAGS) -c readProcesses.c

cpu.o:
	$(CC) $(CFLAGS) -c cpu.c

io.o:
	$(CC) $(CFLAGS) -c io.c

test:
	./a2

leak:
	valgrind --leak-check=yes ./a2

clean:
	rm -rf *.o a2