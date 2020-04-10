#include "RaidStation.h"
#include "CommandManager/CommandManager.h"
#include "CommandHandler/CommandHandler.h"

extern SOCKET g_conn;
static SOCKET g_commandSock, g_commandConnection = INVALID_SOCKET;
static MessageHandler g_messageHandlersArray[eRaidMessageType_Count];
static bool g_isInit = false;
static HANDLE g_commandManagerThreadHandle = INVALID_HANDLE_VALUE;

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
    g_messageHandlersArray[eRaidMessageType_GeneralInfoResponse].validateFunc = Messages_GeneralInfoResponse_Validate;
    g_messageHandlersArray[eRaidMessageType_GeneralInfoResponse].executeFunc = Messages_GeneralInfoResponse_Execute;
    /******************************************/

    //Give the array to the message handler.
    if (MessageHandler_Init(g_messageHandlersArray, eRaidMessageType_Count) != eResult_Success)
    {
        RAID_ERROR("Failed to init message handler");
        return eResult_Failure;
    }

    if (CommandHandler_Init() != eResult_Success)
    {
        RAID_ERROR("Failed to init command handler");
        return eResult_Failure;
    }

    g_isInit = true;
    return eResult_Success;
}

EResult RaidStation_Run()
{
    FD_SET readSet;
    FD_ZERO(&readSet);

    if (g_isInit == false)
    {
        RAID_ERROR("Raid station wasn't initialized");
        return eResult_Failure;
    }

    if (g_conn == INVALID_SOCKET || g_commandConnection == INVALID_SOCKET)
    {
        RAID_ERROR("Received invalid socket");
        return eResult_Failure;
    }

    while (true)
    {
        FD_ZERO(&readSet);
        FD_SET(g_conn, &readSet);
        FD_SET(g_commandConnection, &readSet);

        if (select(0, &readSet, NULL, NULL, NULL) == SOCKET_ERROR)
        {
            RAID_ERROR("Select failed");
            continue;
        }

        if (FD_ISSET(g_conn, &readSet))
        {
            if (raidStation_handleAgentSocketTrigger() != eResult_Success)
            {
                RAID_ERROR("Failed to handle agent socket trigger");
            }
        }

        if (FD_ISSET(g_commandConnection, &readSet))
        {
            if (raidStation_handleCommandSocketTrigger() != eResult_Success)
            {
                RAID_ERROR("Failed to handle command socket trigger");
            }
        }
    }

    return eResult_Success;
}

EResult raidStation_handleAgentSocketTrigger()
{
    int recvSize = 0;
    char buf[sizeof(RaidMessage)] = {0};

    switch(Socket_Recv(g_conn, buf, sizeof(RaidMessage), &recvSize, RAID_STATION_SOCKET_TIMEOUT))
    {
        case (eResult_Success):
        {
            if (raidStation_handleRaidMessage(buf, recvSize) == eResult_Failure)
            {
                RAID_ERROR("Failed to handle raid message");
                return eResult_Failure;
            }
            break;
        }

        case (eResult_Timeout):
        {
            RAID_INFO("Recv timeout.");
            return eResult_Failure;
        }

        case (eResult_Failure):
        {
            RAID_ERROR("Recv failed");
            return eResult_Failure;
        }

        default:
        {
            RAID_ERROR("Unexpected return value from Socket_Recv");
            return eResult_Failure;
        }
    }

    return eResult_Success;
}

EResult raidStation_handleCommandSocketTrigger()
{
    int recvSize = 0;
    char buf[COMMAND_MANAGER_MAX_COMMAND_SIZE] = {0};

    switch(Socket_Recv(g_commandConnection, buf, COMMAND_MANAGER_MAX_COMMAND_SIZE, &recvSize, RAID_STATION_SOCKET_TIMEOUT))
    {
        case (eResult_Success):
        {
            if (raidStation_handleCommand(buf, recvSize) != eResult_Success)
            {
                RAID_ERROR("Failed to handle command %s", buf);
                return eResult_Failure;
            }

            return eResult_Success;
        }

        case (eResult_Timeout):
        {
            RAID_INFO("Recv timeout.");
            return eResult_Failure;
        }

        case (eResult_Failure):
        {
            RAID_ERROR("Recv failed");
            return eResult_Failure;
        }

        default:
        {
            RAID_ERROR("Unexpected return value from Socket_Recv");
            return eResult_Failure;
        }
    }

    return eResult_Success;
}

EResult raidStation_handleCommand(const char* buf, int size)
{
    return CommandHandler_HandleCommand(buf, size);
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
    if (g_commandManagerThreadHandle != INVALID_HANDLE_VALUE)
    {
        RAID_ERROR("Thread handle already initialized.");
        return eResult_Failure;
    }

    int threadID;
    g_commandManagerThreadHandle = CreateThread(NULL, // security attributes ( default if NULL )
                               0, // stack SIZE default if 0
                               CommandManager_ThreadEntry, // Start Address
                               NULL, // input data
                               0, // creational flag ( start if  0 )
                               &threadID); // thread ID

    if (g_commandManagerThreadHandle == INVALID_HANDLE_VALUE)
    {
        RAID_ERROR("Create command manager thread failed");
        return eResult_Failure;
    }

    if (Socket_OpenServerSocket(&g_commandSock, &g_commandConnection, STATION_COMMAND_PORT) != eResult_Success)
    {
        RAID_ERROR("Failed to open command socket");
        return eResult_Failure;
    }

    return eResult_Success;
}

EResult RaidStation_SendMessage(ERaidMessageType messageType, const char* payload, int payloadSize)
{
    if (RaidProtocol_SendMessage(g_conn, messageType, payload, payloadSize) != eResult_Success)
    {
        RAID_ERROR("Failed to send message");
        return eResult_Failure;
    }

    return eResult_Success;
}