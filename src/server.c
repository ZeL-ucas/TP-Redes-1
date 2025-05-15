#include "headers/connectionFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSZ 1024

void Usage(int argc, char **argv) {
    printf("usage: %s <sv4>|| <v6>\n", argv[0]);
    printf("exemple: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        Usage(argc, argv);
    }
    struct sockaddr_storage storage;

    if (ServerSockaddrInit(argv[1], argv[2], &storage) != 0) {
        Usage(argc, argv);
    }
    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);

    if (s == -1) {
        LogExit("socket");
    }
    // permite reutilizar o endereço mesmo c ja tiver sido usado
    int enable = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) != 0) {
        LogExit("setsocketopt");
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    // bind->listen->accept
    if (bind(s, addr, sizeof(storage)) != 0) {
        LogExit("bind");
    }
    // segundo argumento sao o numero de conexoes pendentes
    if (listen(s, 10) != 0) {
        LogExit("listen");
    }

    char addrstr[BUFSZ];
    AddrToString(addr, addrstr, BUFSZ);
    printf(
        "Servidor iniciado em modo IP%s na porta %s. Aguardando conexão...\n",
        argv[1], argv[2]);
    struct sockaddr_storage clientStorage;
    struct sockaddr *clientAddr = (struct sockaddr *)(&storage);
    socklen_t clientAddrLen = sizeof(clientStorage);
    // s = nosso socket
    // clientAddr = endereço do cliente que a funçao sabe de onde veio
    // retorna o socket do cliente
    int clientSocket = accept(s, clientAddr, &clientAddrLen);
    if (clientSocket == -1) {
        LogExit("accept");
    }
    printf("Client Conectado\n");
    char buf[BUFSIZ];
    while (1) {

        memset(buf, 0, BUFSZ);
        ssize_t count = recv(clientSocket, buf, BUFSZ - 1, 0);
        if (count <= 0) {
            LogExit("recv");
        }
        printf("Cliente: %s", buf);
        printf("Cliente: %s", buf);
        printf("Servidor: ");
        send(clientSocket, buf, strlen(buf), 0);
    }

    exit(EXIT_SUCCESS);
}