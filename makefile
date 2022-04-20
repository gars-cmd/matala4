FLAG = -Wall -g

all: server.o stack.o client.o client run

server.o: server.c 
	gcc $(FLAG) -c server.c

client.o: client.c
	gcc $(FLAG) -c client.c

stack.o: stack.c
	gcc $(FLAG) -c stack.c

client: client.o
	gcc $(FLAG) -o client client.o

run: server.o stack.o
	gcc $(FLAG) -lpthread -o run server.o stack.o

.PHONY: clean all

clean:
	rm -f *.o run 
	