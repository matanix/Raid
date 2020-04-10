#include "CommandHandlers.h"
#include "Communication/RaidProtocol/RaidProtocol.h"
#include "Communication/RaidProtocol/MessageStructures.h"
#include "RaidStation/RaidStation.h"

EResult CommandHandler_GeneralInfoRequest(const char* payload, int payloadSize)
{
    return RaidStation_SendMessage(eRaidMessageType_GeneralInfoRequest, NULL, 0);
}

EResult CommandHandler_Shutdown(const char* payload, int payloadSize)
{
    RAID_INFO("Shutting down");
    exit(0);
    return eResult_Success;
}

EResult CommandHandler_Log(const char* payload, int payloadSize)
{
    RAID_INFO("Place holder");
    return eResult_Success;
}

EResult CommandHandler_Help(const char* payload, int payloadSize)
{
    RAID_PRINTLN("Welcome to the raid station.");
    RAID_PRINTLN("Available commands:");
    RAID_PRINTLN("shutdown - shut the station down");
    RAID_PRINTLN("log - print logs to the screen");
    RAID_PRINTLN("geninf - ask for general info from agent");

    return eResult_Success;
}