all: 
	mkdir -p bin
	mkdir -p src/obj
	gcc -Wall -c src/functions/connectionFunctions.c -o src/obj/connectionFunctions.o
	gcc -Wall -c src/functions/gameFunctions.c -o src/obj/gameFunctions.o
	gcc -Wall src/client.c src/obj/connectionFunctions.o -o bin/client
	gcc -Wall src/server.c src/obj/connectionFunctions.o -o bin/server

clean:
	rm -rf bin/
	rm -rf src/obj/

run: all
	gnome-terminal -- bash -c "./bin/server v4 51511; exec bash"
	gnome-terminal -- bash -c "./bin/client 127.0.0.1 51511; exec bash"

clang:
	clang-format -i ./src/client.c
	clang-format -i ./src/server.c
	clang-format -i ./src/functions/connectionFunctions.c
	clang-format -i ./src/functions/gameFunctions.c