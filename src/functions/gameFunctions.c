#include "../headers/gameFunctions.h"

void EnumToString(GameMessage *currentMessage) {
    switch (currentMessage->type) {
    case 0:
        strcpy(currentMessage->message,
               "Escolha sua jogada:\n0 - Nuclear Attack\n1 - Intercept "
               "Attack\n2 - Cyber Attack\n3 - Drone Strike\n4 - Bio Attack");
        break;
    case 1:
        char intToString[10];
        char temp[256];

        sprintf(intToString, "%d", currentMessage->client_action);
        strcpy(temp, "Client Escolheu ");
        strcat(temp, intToString);
        strcpy(currentMessage->message, temp);

        break;
    case 2:
        strcpy(currentMessage->message, "Mensagem para MSG_REQUEST");
        break;
    case 3:
        strcpy(currentMessage->message, "Mensagem para MSG_REQUEST");
        break;
    case 4:
        strcpy(currentMessage->message, "Mensagem para MSG_REQUEST");
        break;
    case 5:
        strcpy(currentMessage->message, "Mensagem para MSG_REQUEST");
        break;
    case 6:
        char clientWinsToString[10];
        char serverWinsToString[10];
        sprintf(clientWinsToString, "%d", currentMessage->client_wins);
        sprintf(serverWinsToString, "%d", currentMessage->server_wins);
        strcpy(currentMessage->message, "Mensagem para MSG_REQUEST");
        break;
    }
}
int CheckError(int value ){
    return !(value <=4 || value >=0) ;
}