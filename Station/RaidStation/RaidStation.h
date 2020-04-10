#ifndef STATION_RAIDSTATION_RAIDSTATION_H
#define STATION_RAIDSTATION_RAIDSTATION_H

#include "Communication/RaidProtocol/RaidProtocol.h"
#include "MessageHandler/MessageHandler.h"

#define RAID_STATION_SOCKET_TIMEOUT     (15)

/** Public RaidStation functions **/
EResult RaidStation_Init();
EResult RaidStation_Run();
EResult RaidStation_SendMessage(ERaidMessageType messageType, const char* payload, int payloadSize);
/****************************/

/** Private RaidStation functions **/
EResult raidStation_handleCommand(const char* buf, int size);
EResult raidStation_handleRaidMessage(const char* buf, int size);
EResult raidStation_createCommandManagerThread();
EResult raidStation_handleCommandSocketTrigger();
EResult raidStation_handleAgentSocketTrigger();
/*****************************/

#endif //STATION_RAIDSTATION_RAIDSTATION_H