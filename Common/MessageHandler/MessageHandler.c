#include "MessageHandler.h"

static MessageHandler* g_messageHandlersArray = NULL;
static int g_messageHandlersArrayLength = 0;
static bool g_isInit = false;

EResult MessageHandler_Init(MessageHandler* messageHandlersArray, int arrayLength)
{
    if (messageHandlersArray == NULL)
    {
        RAID_ERROR("Message handlers array is NULL");
        return eResult_Failure;
    }

    g_messageHandlersArray = messageHandlersArray;
    g_messageHandlersArrayLength = arrayLength;

    g_isInit = true;

    return eResult_Success;
}

EResult MessageHandler_Handle(char* payload, int size, int index)
{
    if (g_isInit == false)
    {
        RAID_ERROR("MessageHandler is not init");
        return eResult_Failure;
    }

    if (index > g_messageHandlersArrayLength)
    {
        RAID_ERROR("Index overflows array length. %d", index);
        return eResult_Failure;
    }

    if (index < 0)
    {
        RAID_ERROR("Negative index %d", index);
        return eResult_Failure;
    }

    if (g_messageHandlersArray[index].validateFunc == NULL || g_messageHandlersArray[index].executeFunc == NULL)
    {
        RAID_INFO("Skipping unsupported message index %d", index);
        return eResult_Success;
    }

    //we do not check for NULL payload or size 0 because the message could be empty.
    if (g_messageHandlersArray[index].validateFunc(payload, size) != eResult_Success)
    {
        RAID_ERROR("Failed to validate message");
        return eResult_Failure;
    }

    if (g_messageHandlersArray[index].executeFunc(payload, size) != eResult_Success)
    {
        RAID_ERROR("Failed to execute message");
        return eResult_Failure;
    }

    return eResult_Success;
}

