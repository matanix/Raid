#ifndef COMMON_COMMUNICATION_RAIDPROTOCOL_RAIDPROTOCOL_H
#define COMMON_COMMUNICATION_RAIDPROTOCOL_RAIDPROTOCOL_H

#include "Communication/Socket/Socket.h"

#define RAID_PROTOCOL_SANITY_SERVER_HELLO "DEADBEEF"
#define RAID_PROTOCOL_SANITY_CLIENT_HELLO "BEEFDEAD"
#define SANITY_TIMEOUT_SEC (5)
#define RAID_PROTOCOL_MESSAGE_PAYLOAD_MAX_SIZE (1000)
#define RAID_PROTOCOL_MESSAGE_MAGIC (0x827ac)

#pragma pack(push, 1)

typedef enum
{
    eRaidMessageType_Min = 0,
    eRaidMessageType_KeepAlive = eRaidMessageType_Min,
    eRaidMessageType_GeneralInfo,
    eRaidMessageType_Max = eRaidMessageType_GeneralInfo
} RaidMessageType;


typedef struct
{
    int magic;
    RaidMessageType messageType;
    SYSTEMTIME time;
    int payloadSize;
} RaidMessageHeader;


typedef struct
{
    RaidMessageHeader header;
    char payload[RAID_PROTOCOL_MESSAGE_PAYLOAD_MAX_SIZE];
} RaidMessage;
#pragma pack(pop)

/** Public RaidProtocol functions **/
EResult RaidProtocol_ClientSanity(SOCKET mySock);
EResult RaidProtocol_ServerSanity(SOCKET mySock);
EResult RaidProtocol_VerifyMessage(const char* buf, int size);
EResult RaidProtocol_SendKeepAlive(SOCKET mySock);
/****************************/

/** Private RaidProtocol functions **/
EResult raidProtocol_sendMessage(SOCKET mySock, RaidMessageType messageType, int payloadSize, const char* payload);
/*****************************/

#endif //COMMON_COMMUNICATION_RAIDPROTOCOL_RAIDPROTOCOL_H