all: 
	mkdir -p bin
	mkdir -p src/obj
	gcc -Wall -c src/functions/connectionFunctions.c -o src/obj/connectionFunctions.o
	gcc -Wall src/client.c src/obj/connectionFunctions.o -o bin/client
	gcc -Wall src/server.c src/obj/connectionFunctions.o -o bin/server

clean:
	rm -rf bin/
	rm -rf src/obj/