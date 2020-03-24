#include "RaidProtocol.h"

EResult RaidProtocol_ClientSanity(SOCKET mySock)
{
    if (Socket_Send(mySock, RAID_PROTOCOL_SANITY_CLIENT_HELLO, sizeof(RAID_PROTOCOL_SANITY_CLIENT_HELLO)) != eResult_Success)
    {
        RAID_ERROR("Failed to send RAID_PROTOCOL_SANITY_CLIENT_HELLO");
        return eResult_Failure;
    }

    char recvBuf[sizeof(RAID_PROTOCOL_SANITY_SERVER_HELLO)] = {NULL_TERMINATOR};
    int recvResult = 0;

    if (Socket_Recv(mySock, recvBuf, sizeof(recvBuf), &recvResult, SANITY_TIMEOUT_SEC) != eResult_Success)
    {
        RAID_ERROR("Failed to recv data");
        return eResult_Failure;
    }

    if (recvResult != sizeof(RAID_PROTOCOL_SANITY_SERVER_HELLO))
    {
        RAID_ERROR("sanity recvResult incorrect size: %d", recvResult);
        return eResult_Failure;
    }


    if (strncmp(recvBuf, RAID_PROTOCOL_SANITY_SERVER_HELLO, recvResult) != 0)
    {
        RAID_ERROR("recvBuf is not equal to RAID_PROTOCOL_SANITY_CLIENT_HELLO");
        return eResult_Failure;
    }

    return eResult_Success;
}

EResult RaidProtocol_ServerSanity(SOCKET mySock)
{
    if (Socket_Send(mySock, RAID_PROTOCOL_SANITY_SERVER_HELLO, sizeof(RAID_PROTOCOL_SANITY_CLIENT_HELLO)) != eResult_Success)
    {
        RAID_ERROR("Failed to send RAID_PROTOCOL_SANITY_CLIENT_HELLO");
        return eResult_Failure;
    }

    char recvBuf[sizeof(RAID_PROTOCOL_SANITY_CLIENT_HELLO)] = {NULL_TERMINATOR};
    int recvResult = 0;

    if (Socket_Recv(mySock, recvBuf, sizeof(recvBuf), &recvResult, SANITY_TIMEOUT_SEC) != eResult_Success)
    {
        RAID_ERROR("Failed to recv data");
        return eResult_Failure;
    }

    if (recvResult != sizeof(RAID_PROTOCOL_SANITY_CLIENT_HELLO))
    {
        RAID_ERROR("sanity recvResult incorrect size: %d", recvResult);
        return eResult_Failure;
    }


    if (strncmp(recvBuf, RAID_PROTOCOL_SANITY_CLIENT_HELLO, recvResult) != 0)
    {
        RAID_ERROR("recvBuf is not equal to RAID_PROTOCOL_SANITY_CLIENT_HELLO");
        return eResult_Failure;
    }

    return eResult_Success;
}

EResult raidProtocol_sendMessage(SOCKET mySock, RaidMessageType messageType, int payloadSize, const char* payload)
{
    RaidMessage message;
    memset(&message, 0, sizeof(message));

    message.header.messageType = messageType;
    message.header.payloadSize = payloadSize;
    GetLocalTime(&message.header.time);

    memcpy(message.payload, payload, payloadSize);

    if (Socket_Send(mySock, &message, sizeof(message.header) + message.header.payloadSize) != eResult_Success)
    {
        RAID_ERROR("Failed to send message");
        return eResult_Failure;
    }

    return eResult_Success;
}