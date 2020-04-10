//
// Created by Matan on 04/04/2020.
//

#ifndef RAID_COMMANDHANDLERS_H
#define RAID_COMMANDHANDLERS_H

#include "CommandHandler.h"

EResult CommandHandler_GeneralInfoRequest(const char* payload, int payloadSize);
EResult CommandHandler_Shutdown(const char* payload, int payloadSize);
EResult CommandHandler_Log(const char* payload, int payloadSize);
EResult CommandHandler_Help(const char* payload, int payloadSize);

#endif //RAID_COMMANDHANDLERS_H
