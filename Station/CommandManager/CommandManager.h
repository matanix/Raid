#ifndef STATION_COMMANDMANAGER_COMMANDMANAGER_H
#define STATION_COMMANDMANAGER_COMMANDMANAGER_H

#include "Typedefs.h"
#include "Log.h"

#define COMMAND_MANAGER_MAX_COMMAND_SIZE (50)
#define STATION_COMMAND_PORT (9082)

/** Public CommandManager functions **/
void CommandManager_ThreadEntry();
/****************************/

/** Private CommandManager functions **/
EResult commandManager_getCommand(char* o_command, int* o_commandSize);
EResult commandManager_verifyCommand(char command, int commandSize);
EResult commandManager_sendCommandToStation(char command, int commandSize);
/*****************************/

#endif //STATION_COMMANDMANAGER_COMMANDMANAGER_H