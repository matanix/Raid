#include "CommandHandlers.h"

static bool g_isInit = false;

CommandNameToHandler g_namesToHandlers[] =
{
        {"geninf", NULL},
        {"log", NULL},
        { "shutdown", NULL},
        { "help", NULL}
};

EResult CommandHandler_Init()
{
    if (g_isInit == true)
    {
        RAID_ERROR("Already init");
        return eResult_Failure;
    }

    g_namesToHandlers[0].handler = CommandHandler_GeneralInfoRequest;
    g_namesToHandlers[1].handler = CommandHandler_Log;
    g_namesToHandlers[2].handler = CommandHandler_Shutdown;
    g_namesToHandlers[3].handler = CommandHandler_Help;

    g_isInit = true;
    return eResult_Success;
}

EResult CommandHandler_HandleCommand(const char* command, int size)
{
    if (g_isInit == false)
    {
        RAID_ERROR("Not init");
        return eResult_Failure;
    }

    if (command == NULL)
    {
        RAID_ERROR("Command is NULL");
        return eResult_Failure;
    }

    if (size <= 0)
    {
        RAID_ERROR("Invalid size: %d", size);
        return eResult_Failure;
    }

    int parseIndex = 0;
    char commandName[COMMAND_MAX_COMMAND_SIZE] = {0};

    if (CommandHandler_ParseWord(command, size, commandName, COMMAND_MAX_COMMAND_SIZE, &parseIndex) != eResult_Success)
    {
        RAID_ERROR("Failed to parse word");
        return eResult_Failure;
    }

    t_specificCommandHandler handler = NULL;

    if (commandHandler_findHandler(commandName, COMMAND_MAX_COMMAND_SIZE, &handler) != eResult_Success)
    {
        RAID_ERROR("didn't find handler for %s command", commandName);
        return eResult_Failure;
    }

    if (handler == NULL)
    {
        RAID_ERROR("%s handler is NULL. Not executing.", commandName);
        return eResult_Success;
    }

    if (handler(command + parseIndex, size - parseIndex) != eResult_Success)
    {
        RAID_ERROR("Failed to handle %s command", commandName);
        return eResult_Failure;
    }

    return eResult_Success;
}

EResult CommandHandler_ParseWord(const char* command, int size, char* o_word, int outSize, int* o_parseIndex)
{
    if (g_isInit == false)
    {
        RAID_ERROR("Not init");
        return eResult_Failure;
    }

    if (outSize == 0 || command == NULL || o_word == NULL || size == 0 || o_parseIndex == NULL)
    {
        RAID_ERROR("Invalid parameter");
        return eResult_Failure;
    }

    int inIndex = 0;
    int outIndex = 0;

    while (commandHandler_isSeperator(command[inIndex]) && (command[inIndex] != NULL_TERMINATOR)
            && (inIndex < size))
    {
        inIndex++;
    }

    while ((!commandHandler_isSeperator(command[inIndex])) && (command[inIndex] != NULL_TERMINATOR)
            && (outIndex < outSize - 1) && (inIndex < size))
    {
        o_word[outIndex++] = command[inIndex++];
    }

    o_word[outIndex] = NULL_TERMINATOR;
    *o_parseIndex = inIndex;

    return eResult_Success;
}

bool commandHandler_isSeperator(char c)
{
    if (c == ' ' || c == '\t' || c == '\n')
    {
        return true;
    }

    return false;
}

EResult commandHandler_findHandler(const char* commandName, int size, t_specificCommandHandler* o_handler)
{
    if (commandName == NULL || size == 0 || o_handler == NULL)
    {
        RAID_ERROR("Invalid parameters");
        return eResult_Failure;
    }

    for (int i = 0; i < sizeof(g_namesToHandlers) / sizeof(CommandNameToHandler); i++)
    {
        if (strncmp(commandName, g_namesToHandlers[i].string, size) == 0)
        {
            *o_handler = g_namesToHandlers[i].handler;
            return eResult_Success;
        }
    }

    return eResult_Failure;
}
