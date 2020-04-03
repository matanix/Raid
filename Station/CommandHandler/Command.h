#ifndef STATION_COMMANDPARSER_COMMAND
#define STATION_COMMANDPARSER_COMMAND

#include "Typedefs.h"
#include "Log.h"
#include "Communication/RaidProtocol/RaidProtocol.h"

#define COMMAND_MAX_TYPE_SIZE       (10)
#define COMMAND_MAX_PARAMETER_SIZE  (50)
#define COMMAND_MAX_COMMAND_SIZE    (10)
#define COMMAND_MAX_PARAMETER_COUNT (10)

typedef enum
{
    eCommandType_Local = 0,
    eCommandType_Remote
} ECommandType;

typedef enum
{
    eLocalCommands_Shutdown = 0,
    eLocalCommands_PrintLog
} ELocalCommand;

#endif //STATION_COMMANDPARSER_COMMAND
