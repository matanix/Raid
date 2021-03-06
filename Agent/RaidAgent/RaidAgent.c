#include "RaidAgent.h"

extern SOCKET g_sock;
static SYSTEMTIME g_lastKA;
static MessageHandler g_messageHandlersArray[eRaidMessageType_Count];
static bool g_isInit = false;

#define FILE_TIME_TO_SECONDS_MUL 10000000LLU

EResult RaidAgent_Init()
{
    //Set to null handlers.
    memset(g_messageHandlersArray, 0, sizeof(g_messageHandlersArray));

    /** Initialize the raid message handlers. **/
    g_messageHandlersArray[eRaidMessageType_GeneralInfoRequest].validateFunc = Messages_GeneralInfoRequest_Validate;
    g_messageHandlersArray[eRaidMessageType_GeneralInfoRequest].executeFunc = Messages_GeneralInfoRequest_Execute;
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

EResult RaidAgent_Run()
{
    if (g_isInit == false)
    {
        RAID_ERROR("Raid agent wasn't initialized");
        return eResult_Failure;
    }

    int recvSize = 0;
    char buf[sizeof(RaidMessage)] = {0};

    if (g_sock == INVALID_SOCKET)
    {
        RAID_ERROR("Received invalid socket");
        return eResult_Failure;
    }

    GetLocalTime(&g_lastKA);

    while (true)
    {
        memset(buf, 0, sizeof(RaidMessage));
        recvSize = 0;
        switch(Socket_Recv(g_sock, buf, sizeof(RaidMessage), &recvSize, RAIDAGENT_KEEPALIVE_TIME))
        {
            case (eResult_Success):
            {
                if (raidAgent_handleRaidMessage(buf, recvSize) == eResult_Failure)
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
                break;
            }
        }

        if (raidAgent_updateKATime() != eResult_Success)
        {
            RAID_ERROR("Failed to update / handle KA time");
        }
    }

    return eResult_Success;
}

EResult raidAgent_handleRaidMessage(const char* buf, int size)
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

EResult raidAgent_updateKATime()
{
    SYSTEMTIME curTime;
    GetLocalTime(&curTime);

    FILETIME curFileTime, lastKAFileTime;

    if (SystemTimeToFileTime(&curTime, &curFileTime) == 0)
    {
        RAID_ERROR("Failed to convert curTime to file time");
        return eResult_Failure;
    }

    if (SystemTimeToFileTime(&g_lastKA, &lastKAFileTime) == 0)
    {
        RAID_ERROR("Failed to convert g_lastKA to file time");
        return eResult_Failure;
    }

    if ((((ULARGE_INTEGER *)&curFileTime)->QuadPart - ((ULARGE_INTEGER *)&lastKAFileTime)->QuadPart) / FILE_TIME_TO_SECONDS_MUL >= RAIDAGENT_KEEPALIVE_TIME)
    {
        RAID_INFO("RAIDAGENT_KEEPALIVE_TIME has passed since last KA - Sending keep alive");
        memcpy(&g_lastKA, &curTime, sizeof(SYSTEMTIME));

        if (RaidProtocol_SendKeepAlive(g_sock) != eResult_Success)
        {
            RAID_ERROR("Failed to send keep alive");
            return eResult_Failure;
        }
    }

    return eResult_Success;
}