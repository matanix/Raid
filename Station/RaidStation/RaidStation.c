#include "RaidStation.h"

extern SOCKET g_sock;
static MessageHandler g_messageHandlersArray[eRaidMessageType_Count];
static bool g_isInit = false;

EResult RaidStation_Init()
{
    if (raidStation_createCommandManagerThread() != eResult_Success)
    {
        RAID_ERROR("Failed to create input manager thread");
        return eResult_Failure;
    }

    //Set to null handlers.
    memset(g_messageHandlersArray, 0, sizeof(g_messageHandlersArray));

    /** Initialize the raid message handlers. **/
    /******************************************/

    //Give the array to the message handler.
    if (MessageHandler_Init(g_messageHandlersArray, eRaidMessageType_Count) != eResult_Success)
    {
        RAID_ERROR("Failed to init message handler");
        return eResult_Failure;
    }

    g_isInit = true;
    return eResult_Success;
}

EResult RaidStation_Run()
{
    if (g_isInit == false)
    {
        RAID_ERROR("Raid station wasn't initialized");
        return eResult_Failure;
    }

    int recvSize = 0;
    char buf[sizeof(RaidMessage)] = {0};

    if (g_sock == INVALID_SOCKET)
    {
        RAID_ERROR("Received invalid socket");
        return eResult_Failure;
    }

    while (true)
    {
        memset(buf, 0, sizeof(RaidMessage));
        recvSize = 0;
        switch(Socket_Recv(g_sock, buf, sizeof(RaidMessage), &recvSize, RAIDSTATION_SOCKET_TIMEOUT))
        {
            case (eResult_Success):
            {
                if (raidStation_handleRaidMessage(buf, recvSize) == eResult_Failure)
                {
                    RAID_ERROR("Failed to handle raid message");
                }
                break;
            }

            case (eResult_Timeout):
            {
                RAID_INFO("Recv timeout.");
                break;
            }

            case (eResult_Failure):
            {
                RAID_ERROR("Recv failed");
                break;
            }

            default:
            {
                RAID_ERROR("Unexpected return value from Socket_Recv");
            }
        }
    }

    return eResult_Success;
}

EResult raidStation_handleRaidMessage(const char* buf, int size)
{
    if (buf == NULL)
    {
        RAID_ERROR("Buf is null");
        return eResult_Failure;
    }

    if (RaidProtocol_VerifyMessage(buf, size) != eResult_Success)
    {
        RAID_ERROR("Message failed to verify");
        return eResult_Failure;
    }

    RaidMessage* message = (RaidMessage*) buf;
    char* payloadPtr = NULL;

    if (message->header.payloadSize != 0)
    {
        payloadPtr = message->payload;
    }

    if (MessageHandler_Handle(payloadPtr, message->header.payloadSize, message->header.messageType) != eResult_Success)
    {
        RAID_ERROR("Failed to handle message of type %d", message->header.messageType);
        return eResult_Failure;
    }

    return eResult_Success;
}

EResult raidStation_createCommandManagerThread()
{
    //Create input manager thread. create an IPC and use it in the main thread for command managing
    return eResult_Success;
}