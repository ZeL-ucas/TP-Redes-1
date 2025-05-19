#include "headers/connectionFunctions.h"
#include "headers/gameFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
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
    // faz com que cada gameplay tenha numeros aleatorios sem repetir
    srand(time(NULL));
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

    printf(
        "Servidor iniciado em modo IP%s na porta %s. Aguardando conexão...\n",
        argv[1], argv[2]);
    struct sockaddr_storage clientStorage;
    struct sockaddr *clientAddr = (struct sockaddr *)(&storage);
    socklen_t clientAddrLen = sizeof(clientStorage);
    while (1) {
        // s = nosso socket
        // clientAddr = endereço do cliente que a funçao sabe de onde veio
        // retorna o socket do cliente
        int clientSocket = accept(s, clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            LogExit("accept");
        }
        printf("Cliente Conectado\n");
        printf("Apresentando as opções para o cliente.\n");
        int connection = 1;
        GameMessage mainMessage;
        InitializateMainMessage(&mainMessage);
        StartGame(clientSocket, &mainMessage);
        // switch que checa qual mensagem chegou e realiza as computaçoes
        // necessarias para o fluxo do jogo
        while (connection) {
            switch (mainMessage.type) {

            case 1:
                // checa se o numero é valido, caso nao seja manda a mensagem de
                // erro e reinicia a rodada de ataque mandando as opçoes denovo
                // ao cliente
                printf("%s\n", mainMessage.message);
                if (!(mainMessage.client_action <= 4 &&
                      mainMessage.client_action >= 0)) {
                    printf("Erro: opção inválida de jogada.\n");
                    CreateErrorMessage(clientSocket, &mainMessage,
                                       "Por favor, selecione um valor de 0 a 4",
                                       MSG_REQUEST);
                    break;
                };

                mainMessage.result = PlayGame(&mainMessage);
                mainMessage.type = MSG_RESULT;
                EnumToString(&mainMessage);
                // depois de gerar o numero verifica, caso seja empate reenvia a
                // lista de opçoes ao cliente, caso contrario soma ao placar e
                // pergunta se o cliente quer jogar denovo
                if (mainMessage.result == 0) {
                    printf("Jogo empatado.\n");
                    strcat(mainMessage.message, "Resultado: Empate!");
                    send(clientSocket, &mainMessage, sizeof(GameMessage), 0);
                    printf("Solicitando ao cliente mais uma escolha.\n");
                    StartGame(clientSocket, &mainMessage);
                } else {
                    if (mainMessage.result == -1) {
                        strcat(mainMessage.message, "Resultado: Vitoria!");
                        mainMessage.client_wins++;
                    } else {
                        strcat(mainMessage.message, "Resultado: Derrota!");
                        mainMessage.server_wins++;
                    }
                    printf("Placar atualizado: Cliente %d x %d Servidor \n",
                           mainMessage.client_wins, mainMessage.server_wins);
                    send(clientSocket, &mainMessage, sizeof(GameMessage), 0);
                    mainMessage.type = MSG_PLAY_AGAIN_REQUEST;
                    printf(
                        "Perguntando se o cliente deseja jogar novamente.\n");
                    EnumToString(&mainMessage);
                    send(clientSocket, &mainMessage, sizeof(GameMessage), 0);
                    recv(clientSocket, &mainMessage, sizeof(GameMessage), 0);
                }
                break;

            case 4:
                // checa se o numero e valido e pede ao cliente pra digitar
                // novamente caso nao seja
                if (!(mainMessage.client_action <= 1 &&
                      mainMessage.client_action >= 0)) {
                    printf("Erro: resposta inválida para jogar novamente.\n");
                    CreateErrorMessage(
                        clientSocket, &mainMessage,
                        "Por favor, digite 1 para jogar novamente "
                        "ou 0 para encerrar.",
                        MSG_PLAY_AGAIN_REQUEST);
                    recv(clientSocket, &mainMessage, sizeof(GameMessage), 0);
                    break;
                };
                EnumToString(&mainMessage);
                printf("%s\n", mainMessage.message);
                // caso cliente nao queira jogar, manda o placar atualizado e
                // fecha a conexao tanto do cliente quanto do servidor
                if (mainMessage.client_action == 0) {
                    mainMessage.type = MSG_END;
                    EnumToString(&mainMessage);
                    printf("Enviando placar final.\n");
                    send(clientSocket, &mainMessage, sizeof(GameMessage), 0);
                    printf("Encerrando conexão.\n");
                    close(clientSocket);
                    printf("Cliente desconectado.\n");
                    connection = 0;
                } else {
                    // caso ele queira jogar reinicia o fluxo desde o começo
                    StartGame(clientSocket, &mainMessage);
                }
                break;
            default:
                break;
            }
        }
    }
    close(s);
    exit(EXIT_SUCCESS);
}