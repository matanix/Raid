//
// Created by Matan on 04/04/2020.
//

#ifndef RAID_GENERALINFORESPONSE_H
#define RAID_GENERALINFORESPONSE_H

#include "Typedefs.h"
#include "Log.h"
#include "Communication/RaidProtocol/MessageStructures.h"

EResult Messages_GeneralInfoResponse_Validate(char* payload, int payloadSize);
EResult Messages_GeneralInfoResponse_Execute(char* payload, int payloadSize);

#endif //RAID_GENERALINFORESPONSE_H
