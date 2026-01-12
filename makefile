.PHONY: compile clean

compile: server.o networking.o client.o game.o
	@gcc -o server server.o networking.o game.o
	@gcc -o client client.o networking.o game.o

game.o: game.c game.h
	@gcc -c game.c

server: server.o game.o
	@gcc -o server server.o networking.o game.o

client: client.o game.o
	@gcc -o client client.o networking.o game.o

server.o: server.c networking.h game.h
	@gcc -c server.c

networking.o: networking.c networking.h game.h
	@gcc -c networking.c

client.o: client.c networking.h game.h
	@gcc -c client.c

clean:
	@rm *.o server client
