#include "headers/connectionFunctions.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define BUFSZ 1024

void Usage(int argc, char **argv) {
    printf("usage: %s <server IP> <server port>", argv[0]);
    printf("example: %s 127.0.0.1 51511", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        Usage(argc, argv);
    }

    struct sockaddr_storage storage;

    if (AddrParser(argv[1], argv[2], &storage) != 0) {
        Usage(argc, argv);
    }
    int s;
    // passa o storage.ss_family para inicializar com o IPV4, ou IPV6
    // corretamente
    s = socket(storage.ss_family, SOCK_STREAM, 0);

    if (s == -1) {
        LogExit("socket");
    }
    struct sockaddr *addr = (struct sockaddr *)(&storage);

    if (0 != connect(s, addr, sizeof(storage))) {
        LogExit("connect");
    }

    char addrstr[BUFSZ];
    AddrToString(addr, addrstr, BUFSZ);

    printf("Conectado ao servidor\n");

    char buf[BUFSZ];

    memset(buf, 0, BUFSZ);
    unsigned total = 0;
    while (1) {

        printf("Você: ");
        if (!fgets(buf, BUFSZ, stdin))
            break;

        send(s, buf, strlen(buf), 0);
        if (strncmp(buf, "exit", 4) == 0)
            break;

        memset(buf, 0, BUFSZ);
        ssize_t count = recv(s, buf, BUFSZ - 1, 0);
        if (count <= 0) {
            LogExit("recv");
        }
        printf("Servidor: %s", buf);
    }
    close(s);

    printf("receive %d bytes \n", total);
    puts(buf);
    exit(EXIT_SUCCESS);
}