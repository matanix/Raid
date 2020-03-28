#include "CommandManager.h"
#include <stdio.h>
#include <string.h>
#include "Communication/Socket/Socket.h"

static SOCKET g_stationSock = INVALID_SOCKET;

void CommandManager_ThreadEntry()
{
    if (Socket_OpenClientSocket(g_stationSock, STATION_COMMAND_PORT) != eResult_Success)
    {
        RAID_ERROR("Failed to open command manager client socket");
        return;
    }

    char command[COMMAND_MANAGER_MAX_COMMAND_SIZE] = {0};
    int commandSize = 0;

    while (true)
    {
        commandSize = 0;
        memset(command, 0, sizeof(command));
        if (commandManager_getCommand(command, &commandSize) != eResult_Success)
        {
            RAID_ERROR("Failed to get command from user");
            continue;
        }

        if (commandManager_verifyCommand(command, commandSize) != eResult_Success)
        {
            RAID_ERROR("Failed to verify command.");
            continue;
        }

        if (commandManager_sendCommandToStation(command, commandSize) != eResult_Success)
        {
            RAID_ERROR("Failed to send command to station");
            continue;
        }
    }
}

EResult commandManager_getCommand(char* o_command, int* o_commandSize)
{
    return eResult_Success;
}
EResult commandManager_verifyCommand(char command, int commandSize)
{
    return eResult_Success;
}
EResult commandManager_sendCommandToStation(char command, int commandSize)
{
    return eResult_Success;
}
