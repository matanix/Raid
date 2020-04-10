//
// Created by Matan on 04/04/2020.
//

#include "GeneralInfoResponse.h"

EResult Messages_GeneralInfoResponse_Validate(char* payload, int payloadSize)
{
    if (payload == NULL || payloadSize != sizeof(GeneralInfoResponse))
    {
        RAID_ERROR("Invalid payload");
        return eResult_Failure;
    }

    return eResult_Success;
}

EResult Messages_GeneralInfoResponse_Execute(char* payload, int payloadSize)
{
    GeneralInfoResponse* resp = (GeneralInfoResponse*) payload;
    RAID_PRINTLN("Printing general info response data");
    RAID_PRINTLN("The username is %s", resp->username);
    RAID_PRINTLN("The IP is %s", resp->ip);

    return eResult_Success;
}