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
    GameMessage mainMessage;
    memset(buf, 0, BUFSZ);

    recv(s, &mainMessage, sizeof(mainMessage), 0);
    int gameRunning = 1;
    // diferente do servidor, cada opçao do switch simplesmente serve para mexer
    // na ordem dos "send e recv", com poucas açoes sendo tomadas desse lado
    while (gameRunning) {

        switch (mainMessage.type) {
        case 0:
            printf("%s\n", mainMessage.message);
            char action[10];
            fgets(action, 10, stdin);

            mainMessage.client_action = atoi(action);
            mainMessage.type = MSG_RESPONSE;

            send(s, &mainMessage, sizeof(mainMessage), 0);
            recv(s, &mainMessage, sizeof(mainMessage), 0);

            break;
        case 2:

            printf("%s\n", mainMessage.message);
            recv(s, &mainMessage, sizeof(mainMessage), 0);

            break;
        case 3:
            printf("%s\n", mainMessage.message);
            fgets(action, 10, stdin);

            mainMessage.client_action = atoi(action);
            mainMessage.type = 4;

            send(s, &mainMessage, sizeof(mainMessage), 0);
            recv(s, &mainMessage, sizeof(mainMessage), 0);

            break;
        case 5:
            printf("%s\n", mainMessage.message);
            recv(s, &mainMessage, sizeof(mainMessage), 0);
            break;
        case 6:
            printf("%s\n", mainMessage.message);
            close(s);
            gameRunning = 0;
            break;
        default:
            LogExit("Comunication Error");
        }
    }

    exit(EXIT_SUCCESS);
}