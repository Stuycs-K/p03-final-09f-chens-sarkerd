.PHONY: compile clean

compile: server.o networking.o client.o
	@gcc -o server server.o networking.o
	@gcc -o client client.o networking.o

server.o: server.c networking.h
	@gcc -c server.c

networking.o: networking.c networking.h
	@gcc -c networking.c

client.o: client.c networking.h
	@gcc -c client.c

clean:
	@rm *.o server client
