#include "GeneralInfoRequest.h"
#include "Communication/RaidProtocol/RaidProtocol.h"
#include "Communication/RaidProtocol/MessageStructures.h"

EResult CommandHandler_GeneralInfoRequest(const char* payload, int payloadSize)
{
    return RaidStation_SendMessage(eRaidMessageType_GeneralInfoRequest, NULL, 0);
}

EResult CommandHandler_Shutdown(const char* payload, int payloadSize)
{
    LOG_INFO("Shutting down");
    exit(0);
    return eResult_Success;
}

EResult CommandHandler_Log(const char* payload, int payloadSize)
{
    LOG_INFO("Place holder");
    return eResult_Success;
}