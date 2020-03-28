#ifndef COMMON_MESSAGEHANDLER_MESSAGEHANDLER_H
#define COMMON_MESSAGEHANDLER_MESSAGEHANDLER_H

#include "Typedefs.h"
#include "Log.h"

typedef EResult (*t_messageHandler_Validate)(char* payload, int payloadSize);
typedef EResult (*t_messageHandler_Execute)(char* payload, int payloadSize);

typedef struct
{
    t_messageHandler_Validate validateFunc;
    t_messageHandler_Execute executeFunc;
} MessageHandler;


/** Public MessageHandler functions **/
EResult MessageHandler_Init(MessageHandler* messageHandlersArray, int arrayLength);
EResult MessageHandler_Handle(char* payload, int size, int index);
/****************************/

/** Private MessageHandler functions **/
/*****************************/

#endif //COMMON_MESSAGEHANDLER_MESSAGEHANDLER_H