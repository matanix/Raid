#ifndef AGENT_RAIDAGENT_RAIDAGENT_H
#define AGENT_RAIDAGENT_RAIDAGENT_H

#include "Communication/RaidProtocol/RaidProtocol.h"

#define RAIDAGENT_KEEPALIVE_TIME (60)

/** Public RaidAgent functions **/
EResult RaidAgent_Run();
/****************************/

/** Private RaidAgent functions **/
EResult raidAgent_handleRaidMessage(const char* buf, int size);
EResult raidAgent_handleTimeout();
/*****************************/

#endif //AGENT_RAIDAGENT_RAIDAGENT_H