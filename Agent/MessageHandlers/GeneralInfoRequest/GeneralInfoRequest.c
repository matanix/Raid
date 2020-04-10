#include "GeneralInfoRequest.h"
#include <Winbase.h>
#include <stdlib.h>
#include <Iphlpapi.h>

#pragma comment(lib, "IPHLPAPI.lib")

extern SOCKET g_sock;

EResult Messages_GeneralInfoRequest_Validate(char* payload, int payloadSize)
{
    if (payload != NULL || payloadSize != 0)
    {
        RAID_ERROR("GeneralInfoRequest should have no payload");
        return eResult_Failure;
    }

    return eResult_Success;
}

EResult Messages_GeneralInfoRequest_Execute(char* payload, int size)
{
    GeneralInfoResponse response;
    memset(&response, 0, sizeof(GeneralInfoResponse));

    if (generalInfoRequest_getUserName(response.username) != eResult_Success)
    {
        RAID_ERROR("Failed to retrieve username");
        return eResult_Failure;
    }

    if (generalInfoRequest_getIP(response.ip) != eResult_Success)
    {
        RAID_ERROR("Failed to retreive IP");
        return eResult_Failure;
    }

    return RaidProtocol_SendMessage(g_sock, eRaidMessageType_GeneralInfoResponse, (char*)&response, sizeof(response));
}

EResult generalInfoRequest_getUserName(char* o_userName)
{
    int inOutSize = GENERAL_INFO_RESPONSE_MAX_USERNAME_SIZE;

    if (GetUserNameA(o_userName, &inOutSize) == 0)
    {
        RAID_ERROR("Failed to get username");
        return eResult_Failure;
    }

    return eResult_Success;
}

EResult generalInfoRequest_getIP(char* o_ip)
{
    RAID_INFO("Placeholder");
    return eResult_Success;
}