#pragma once 

#include <inttypes.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MSG_SIZE 256
/**
 * @enum MessageType
 * @brief Tipos de mensagens usadas na comunicação cliente servidor do jogo.
 */
typedef enum {
    MSG_REQUEST,
    MSG_RESPONSE,
    MSG_RESULT,
    MSG_PLAY_AGAIN_REQUEST,
    MSG_PLAY_AGAIN_RESPONSE,
    MSG_ERROR,
    MSG_END
} MessageType;
/**
 * @struct GameMessage
 * @brief struct principal da troca de mensagens
 *
 * contém informações da jogada, resultado, placar e mensagem de texto.
 */
typedef struct {
    int type;               /**< tipo da mensagem (MessageType) */
    int client_action;      /**< acao atual do cliente*/
    int server_action;      /**< acao atual do servidor */
    int result;             /**< resultado da rodada, 0 empate, 1 servidor,-1 cliente */
    int client_wins;        /**< quantidade de vitórias do cliente */
    int server_wins;        /**< quantidade de vitórias do servidor */
    char message[MSG_SIZE]; /**< mensagem  */
} GameMessage;

void LogExit(char *msg);
/**
 * @brief converte uma string de endereço IP e porta para sockaddr_storage, pega das videoaulas disponibilizadas.
 *
 * @param addrstring string contendo o endereço IP (IPv4 ou IPv6).
 * @param portstr string contendo a porta.
 * @param storage ponteiro para a struct sockaddr_storage a ser preenchida.
 * @return int 0 em sucesso, diferente de 0 em erro.
 */
int AddrParser(const char *addrstring, const char *portstr,
               struct sockaddr_storage *storage);
/**
 * @brief converte um endereço sockaddr em string , pega das videoaulas disponibilizadas.
 *
 * @param addr ponteiro para a struct sockaddr.
 * @param str buffer onde será armazenada a string formatada.
 * @param strsize tamanho do buffer str.
 * @return void
 */
void AddrToString(const struct sockaddr *addr, char *str, size_t strsize);
/**
 * @brief inicializa uma struct sockaddr_storage para servidor, pega das videoaulas disponibilizadas
 *
 * tambem configura porta e IP
 *
 * @param proto string que especifica o protocolo ("v4" ou "v6").
 * @param portstr porta do servidor como string.
 * @param storage ponteiro para sockaddr_storage que será inicializado.
 * @return int 0 em sucesso, diferente de 0 em erro.
 */
int ServerSockaddrInit(const char *proto, const char *portstr,
                       struct sockaddr_storage *storage);
