FLAG = -Wall -g

all: main.o stack.o run

main.o: main.c 
	gcc $(FLAG) -c main.c

stack.o: stack.c
	gcc $(FLAG) -c stack.c

run: main.o stack.o
	gcc $(FLAG) -o run main.o stack.o

.PHONY: clean all

clean:
	rm -f *.o run
	