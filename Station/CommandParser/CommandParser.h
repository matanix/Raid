#ifndef STATION_COMMANDPARSER_COMMANDPARSER
#define STATION_COMMANDPARSER_COMMANDPARSER

#include "Command.h"
#include "Communication/RaidProtocol/RaidProtocol.h"

/** Public CommandParser functions **/
EResult ParseCommand(char* io_command, int size, ParsedCommand* o_parsedCommand);
/****************************/

#endif //STATION_COMMANDPARSER_COMMANDPARSER