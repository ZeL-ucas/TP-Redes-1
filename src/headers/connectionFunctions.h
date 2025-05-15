#pragma once 

#include <inttypes.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MSG_SIZE 256

typedef enum {
    MSG_REQUEST,
    MSG_RESPONSE,
    MSG_RESULT,
    MSG_PLAY_AGAIN_REQUEST,
    MSG_PLAY_AGAIN_RESPONSE,
    MSG_ERROR,
    MSG_END
} MessageType;

typedef struct {
    int type;          
    int client_action; 
    int server_action;
    int result;       
    int client_wins;  
    int server_wins;  
    char message[MSG_SIZE]; 
} GameMessage;
void LogExit(char *msg);

int AddrParser(const char *addrstring, const char *portstr,
               struct sockaddr_storage *storage);

void AddrToString(const struct sockaddr *addr, char *str, size_t strsize);

int ServerSockaddrInit(const char *proto, const char *portstr,
                       struct sockaddr_storage *storage);
