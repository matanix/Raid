#ifndef STATION_COMMANDPARSER_COMMAND
#define STATION_COMMANDPARSER_COMMAND

#include "Typedefs.h"
#include "Log.h"

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

typedef struct
{
    char type[COMMAND_MAX_TYPE_SIZE];
    char command[COMMAND_MAX_COMMAND_SIZE];
    int parameterCount;
    char parameters[COMMAND_MAX_PARAMETER_COUNT][COMMAND_MAX_PARAMETER_SIZE];
} RawCommand;

typedef union
{

} LocalCommandPayload;

typedef union
{

} RemoteCommandPayload;

typedef struct
{
    ELocalCommand command;
    LocalCommandPayload payload;
} LocalCommand;

typedef struct
{
    ERaidMessageType command;
    RemoteCommandPayload payload;
} RemoteCommand;

typedef struct
{
    ECommandType type;
    union
    {
        LocalCommand,
        RemoteCommand
    } payload;
} ParsedCommand;

#endif //STATION_COMMANDPARSER_COMMAND