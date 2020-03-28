#ifndef AGENT_RAIDAGENT_RAIDAGENT_H
#define AGENT_RAIDAGENT_RAIDAGENT_H

#include "Communication/RaidProtocol/RaidProtocol.h"
#include "MessageHandler/MessageHandler.h"
#include "MessageHandlers/GeneralInfoRequest/GeneralInfoRequest.h"

#define RAIDAGENT_KEEPALIVE_TIME (4)

/** Public RaidAgent functions **/
EResult RaidAgent_Init();
EResult RaidAgent_Run();
/****************************/

/** Private RaidAgent functions **/
EResult raidAgent_handleRaidMessage(const char* buf, int size);
EResult raidAgent_updateKATime();
/*****************************/

#endif //AGENT_RAIDAGENT_RAIDAGENT_H