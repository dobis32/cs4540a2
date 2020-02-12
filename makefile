CC=gcc
CFLAGS= -ggdb -Wall -pedantic -std=c11

make: clean readProcesses.o priorityq.o processActions.o io.o queue.o
	$(CC) $(CFLAGS) -o a2 main.c

readProcesses.o:
	$(CC) $(CFLAGS) -c readProcesses.c

priorityq.o:
	$(CC) $(CFLAGS) -c priorityq.c

processActions.o:
	$(CC) $(CFLAGS) -c processActions.c

io.o:
	$(CC) $(CFLAGS) -c io.c

queue.o:
	$(CC) $(CFLAGS) -c queue.c

test:
	./a2

leak:
	valgrind --leak-check=yes ./a2

clean:
	rm -rf *.o a2