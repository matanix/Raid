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

EResult RaidProtocol_SendMessage(SOCKET mySock, ERaidMessageType messageType, const char* payload, int payloadSize)
{
    if (mySock == INVALID_SOCKET)
    {
        RAID_ERROR("Invalid socket.");
        return eResult_Failure;
    }

    if (messageType < eRaidMessageType_Min || messageType > eRaidMessageType_Max)
    {
        RAID_ERROR("Invalid messageType : %d was requested to be sent", messageType);
        return eResult_Failure;
    }

    if ((payloadSize == 0) ^ (payload == NULL))
    {
        RAID_ERROR("Only one payload parameter is empty. size: %d", payloadSize);
        return eResult_Failure;
    }

    RaidMessage message;
    memset(&message, 0, sizeof(message));

    message.header.magic = RAID_PROTOCOL_MESSAGE_MAGIC;
    message.header.messageType = messageType;
    message.header.payloadSize = payloadSize;
    GetLocalTime(&message.header.time);

    if (payload != NULL)
    {
        memcpy(message.payload, payload, payloadSize);
    }

    if (Socket_Send(mySock, (const char*)&message, sizeof(message.header) + message.header.payloadSize) != eResult_Success)
    {
        RAID_ERROR("Failed to send message");
        return eResult_Failure;
    }

    return eResult_Success;
}

EResult RaidProtocol_VerifyMessage(const char* buf, int size)
{
    if (buf == NULL)
    {
        RAID_ERROR("Buf is null");
        return eResult_Failure;
    }

    if (size < sizeof(RaidMessageHeader) || size > sizeof(RaidMessage))
    {
        RAID_ERROR("Invalid message size : %d", size);
        return eResult_Failure;
    }

    RaidMessageHeader* header = (RaidMessageHeader*) buf;

    if (header->magic != RAID_PROTOCOL_MESSAGE_MAGIC)
    {
        RAID_ERROR("Invalid header magic : %d", header->magic);
        return eResult_Failure;
    }

    if (header->messageType < eRaidMessageType_Min || header->messageType > eRaidMessageType_Max)
    {
        RAID_ERROR("Invalid message type : %d", header->messageType);
        return eResult_Failure;
    }

    if (header->payloadSize > RAID_PROTOCOL_MESSAGE_PAYLOAD_MAX_SIZE)
    {
        RAID_ERROR("Too big payload size : %d", header->payloadSize);
        return eResult_Failure;
    }

    return eResult_Success;
}

EResult RaidProtocol_SendKeepAlive(SOCKET mySock)
{
    if (mySock == INVALID_SOCKET)
    {
        RAID_ERROR("Invalid socket.");
        return eResult_Failure;
    }

    return RaidProtocol_SendMessage(mySock, eRaidMessageType_KeepAlive, NULL, 0);
}
