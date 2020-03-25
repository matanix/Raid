#include "RaidAgent.h"

extern SOCKET g_sock;
SYSTEMTIME g_lastKA;

EResult RaidAgent_Run()
{
    int recvSize = 0;
    char buf[sizeof(RaidMessage)] = {0};

    if (g_sock == INVALID_SOCKET)
    {
        RAID_ERROR("Received invalid socket");
        return eResult_Failure;
    }

    memset(&g_lastKA, 0, sizeof(g_lastKA));

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

                if (raidAgent_handleTimeout() == eResult_Failure)
                {
                    RAID_ERROR("Failed to handle timeout");
                }

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

    //Handle message using g_arr of message handlers.

    return eResult_Success;
}

EResult raidAgent_handleTimeout()
{
    if (RaidProtocol_SendKeepAlive(g_sock) != eResult_Success)
    {
        RAID_ERROR("Failed to send keep alive");
        return eResult_Failure;
    }

    GetLocalTime(&g_lastKA);
    return eResult_Success;
}