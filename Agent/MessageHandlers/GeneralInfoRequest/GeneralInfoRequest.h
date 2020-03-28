#ifndef AGENT_MESSAGEHANDLERS_GENERALINFOREQUEST_GENERALINFOREQUEST_H
#define AGENT_MESSAGEHANDLERS_GENERALINFOREQUEST_GENERALINFOREQUEST_H

#include "MessageHandler/MessageHandler.h"
#include "Communication/RaidProtocol/MessageStructures.h"
#include "Communication/RaidProtocol/RaidProtocol.h"

/** Public GeneralInfoRequest functions **/
EResult Messages_GeneralInfoRequest_Validate(char* payload, int payloadSize);
EResult Messages_GeneralInfoRequest_Execute(char* payload, int size);
/****************************/

/** Private GeneralInfoRequest functions **/
EResult generalInfoRequest_getUserName(char* o_userName);
EResult generalInfoRequest_getIP(char* o_ip);
/*****************************/

#endif //AGENT_MESSAGEHANDLERS_GENERALINFOREQUEST_GENERALINFOREQUEST_H