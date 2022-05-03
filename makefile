FLAG = -Wall -g
GC = gcc

all: server.o stack.o client.o client server  

server.o: server.c 
	$(GC) $(FLAG) -c server.c

client.o: client.c
	$(GC) $(FLAG) -c client.c

stack.o: stack.c
	$(GC) $(FLAG) -c stack.c

client: client.o stack.o
	$(GC) $(FLAG) -o client client.o stack.o

server: server.o stack.o
	$(GC) $(FLAG) -lpthread -o server server.o stack.o

.PHONY: clean all

clean:
	rm -f *.o server client test
	
