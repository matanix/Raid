#ifndef COMMON_COMMUNICATION_SOCKET_SOCKET_H
#define COMMON_COMMUNICATION_SOCKET_SOCKET_H

#include "Typedefs.h"
#include "Log.h"
#include <winsock2.h>

#define DEFAULT_PORT (7171)

/** Public socket functions **/
EResult Socket_OpenSocket(SOCKET* o_newSocket);
EResult Socket_Cleanup();
/****************************/

/** Private socket functions **/
EResult socket_initializeWSA();
/*****************************/

#endif //COMMON_COMMUNICATION_SOCKET_SOCKET_H