#ifndef STATION_RAIDSTATION_RAIDSTATION_H
#define STATION_RAIDSTATION_RAIDSTATION_H

#include "Communication/RaidProtocol/RaidProtocol.h"
#include "MessageHandler/MessageHandler.h"

#define RAIDSTATION_SOCKET_TIMEOUT (15)

/** Public RaidStation functions **/
EResult RaidStation_Init();
EResult RaidStation_Run();
/****************************/

/** Private RaidStation functions **/
EResult raidStation_handleRaidMessage(const char* buf, int size);
EResult raidStation_createCommandManagerThread();
/*****************************/

#endif //STATION_RAIDSTATION_RAIDSTATION_H