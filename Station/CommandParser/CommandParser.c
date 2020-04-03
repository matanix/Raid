#include "CommandParser.h"

EResult ParseCommand(const char* command, int size, ParsedCommand* o_parsedCommand)
{
    if (command == NULL)
    {
        RAID_ERROR("Command is NULL");
        return eResult_Failure;
    }

    if (size <= 0)
    {
        RAID_ERROR("Size is too small: %d", size);
        return eResult_Failure;
    }

    if (o_parsedCommand == NULL)
    {
        RAID_ERROR("o_parsedCommand is NULL");
        return eResult_Failure;
    }
}
