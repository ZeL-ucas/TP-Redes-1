
//inclue o connection functions para ter acesso a struct
#include "connectionFunctions.h"

#define gameArray = ["Nuclear Attack","Intercept Attack","Cyber Attack","Drone Strike","Bio Attack"]
/**
 * @brief Monta a mensagem necessaria baseada no type da gameMessage
 *
 * Recebe a gameMessage e usa os dados internos dela com algumas manipulaçoes para criar o message apropriado
 *
 * @param currentMessage mensagem a ser alterada
 * @return void
 */
void EnumToString( GameMessage *currentMessage);


int CheckError (int value);