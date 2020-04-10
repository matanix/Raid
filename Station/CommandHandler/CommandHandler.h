#ifndef STATION_COMMANDHANDLER_COMMANDHANDLER
#define STATION_COMMANDHANDLER_COMMANDHANDLER

#include "Command.h"
#include "Communication/RaidProtocol/RaidProtocol.h"

typedef EResult (*t_specificCommandHandler)(const char* payload, int payloadSize);

/** Public CommandHandler functions **/
EResult CommandHandler_Init();
EResult CommandHandler_HandleCommand(const char* command, int size);
EResult CommandHandler_ParseWord(const char* command, int size, char* o_word, int outSize, int* o_parseIndex);
/****************************/

/** Private CommandHandler functions **/
bool commandHandler_isSeperator(char c);
EResult commandHandler_findHandler(const char* commandName, int size, t_specificCommandHandler* o_handler);
/*************************************/

typedef struct
{
    char string[COMMAND_MAX_COMMAND_SIZE];
    t_specificCommandHandler handler;
} CommandNameToHandler;


#endif //STATION_COMMANDHANDLER_COMMANDHANDLER