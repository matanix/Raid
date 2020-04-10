#include "CommandManager.h"
#include <stdio.h>
#include <string.h>
#include "Communication/Socket/Socket.h"

static SOCKET g_stationSock = INVALID_SOCKET;

unsigned long WINAPI CommandManager_ThreadEntry(void* params)
{
    if (Socket_OpenClientSocket(&g_stationSock, STATION_COMMAND_PORT) != eResult_Success)
    {
        RAID_ERROR("Failed to open command manager client socket");
        return THREAD_ERROR;
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

        if (command[0] == '\n')
        {
            continue;
        }

        //Remove trailing \n
        command[commandSize - 1] = NULL_TERMINATOR;
        commandSize--;

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

    return THREAD_SUCCESS;
}

EResult commandManager_getCommand(char* o_command, int* o_commandSize)
{
    if (fgets(o_command, COMMAND_MANAGER_MAX_COMMAND_SIZE, stdin) == NULL)
    {
        RAID_ERROR("fgets failed.");
        return eResult_Failure;
    }

    *o_commandSize = strnlen(o_command, COMMAND_MANAGER_MAX_COMMAND_SIZE);

    return eResult_Success;
}
EResult commandManager_verifyCommand(char* command, int commandSize)
{
    return eResult_Success;
}
EResult commandManager_sendCommandToStation(char* command, int commandSize)
{
    if (Socket_Send(g_stationSock, command, commandSize) != eResult_Success)
    {
        RAID_ERROR("Failed to send command to station");
        return eResult_Failure;
    }

    return eResult_Success;
}
