ifeq ($(DEBUG),true)
	CC = gcc -g
else
	CC = gcc
endif

all: main.o shell_functions.o
	$(CC) -o program main.o shell_functions.o

main.o: main.c shell_headers.h
	$(CC) -c main.c

shell_functions.o: shell_functions.c shell_headers.h
	$(CC) -c shell_functions.c

run:
	./program

clean:
	rm *.o
	rm *~

memcheck:
	valgrind --leak-check:yes ./program
