#include "../headers/gameFunctions.h"

void EnumToString(GameMessage *currentMessage) {
    const char *gameArray[] = {"Nuclear Attack", "Intercept Attack",
                               "Cyber Attack", "Drone Strike", "Bio Attack"};
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
        sprintf(currentMessage->message,
                "Você escolheu: %s\nServidor escolheu: %s\n",
                gameArray[currentMessage->client_action],
                gameArray[currentMessage->server_action]);
        break;
    case 3:
        strcpy(currentMessage->message,
               "Deseja jogar novamente?\n1- Sim\n0-Nao");
        break;
    case 4:
        if (currentMessage->client_action == 0) {
            strcpy(currentMessage->message,
                   "Cliente não deseja jogar novamente");
        } else {
            strcpy(currentMessage->message, "Cliente deseja jogar novamente.");
        }
        break;
    case 6:
        char clientWinsToString[10];
        char serverWinsToString[10];
        sprintf(clientWinsToString, "%d", currentMessage->client_wins);
        sprintf(serverWinsToString, "%d", currentMessage->server_wins);
        sprintf(currentMessage->message,
                "Fim de jogo!\nPlacar final: Você %d x %d Servidor\nObrigado "
                "por jogar!",
                currentMessage->client_wins, currentMessage->server_wins);
        break;
    }
}

int PlayGame(GameMessage *mainMessage) {
    int serverNumber = rand() % 5;
    printf("Servidor escolheu aleatoriamente %d\n", serverNumber);
    mainMessage->server_action = serverNumber;
    // checa empate
    if (serverNumber != mainMessage->client_action) {
        if (CheckWinner(serverNumber, mainMessage->client_action)) {
            return 1;
        } else {
            return -1;
        };

    } else {
        return 0;
    }
}

int CheckWinner(int server, int player) {
    int winner;
    switch (server) {
    case 0:
        winner = (player != 1 && player != 4);
        break;
    case 1:
        winner = (player != 2 && player != 3);

        break;
    case 2:
        winner = (player != 0 && player != 4);
        break;
    case 3:
        winner = (player != 0 && player != 2);
        break;
    case 4:
        winner = (player != 1 && player != 3);
        break;
    }
    return winner;
}

void InitializateMainMessage(GameMessage *mainMessage) {
    mainMessage->client_action = 0;
    mainMessage->server_action = 0;
    mainMessage->client_wins = 0;
    mainMessage->server_wins = 0;
    mainMessage->result = 0;
    mainMessage->type = 0;
    strcpy(mainMessage->message, "0");
};

void StartGame(int clientSocket, GameMessage *mainMessage) {
    mainMessage->type = MSG_REQUEST;
    EnumToString(mainMessage);
    send(clientSocket, mainMessage, sizeof(GameMessage), 0);
    recv(clientSocket, mainMessage, sizeof(GameMessage), 0);
    EnumToString(mainMessage);
    return;
};

void CreateErrorMessage(int clientSocket, GameMessage *mainMessage,
                        char *errorMessage, int newMessage) {
    mainMessage->type = MSG_ERROR;
    strcpy(mainMessage->message, errorMessage);
    send(clientSocket, mainMessage, sizeof(GameMessage), 0);
    if (newMessage == 0) {
        StartGame(clientSocket, mainMessage);
        return;
    }
    mainMessage->type = newMessage;
    EnumToString(mainMessage);
    send(clientSocket, mainMessage, sizeof(GameMessage), 0);
}