#include "../headers/gameFunctions.h"

const char *EnumToString(MessageType type) {
    switch (type) {
    case MSG_REQUEST:
        return "Escolha sua jogada:\n 0 - Nuclear Attack \n 1 - Intercept "
               "Attack \n 2 - Cyber Attack \n 3 - Drone Strike \n 4 - Bio "
               "Attack$ 2";
    case MSG_RESPONSE:
        return "MSG_RESPONSE";
    case MSG_RESULT:
        return "MSG_RESULT";
    case MSG_PLAY_AGAIN_REQUEST:
        return "MSG_PLAY_AGAIN_REQUEST";
    case MSG_PLAY_AGAIN_RESPONSE:
        return "MSG_PLAY_AGAIN_RESPONSE";
    case MSG_ERROR:
        return "MSG_ERROR";
    case MSG_END:
        return "MSG_END";
    default:
        return "UNKNOWN";
    }
}