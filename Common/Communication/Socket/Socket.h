#ifndef COMMON_COMMUNICATION_SOCKET_SOCKET_H
#define COMMON_COMMUNICATION_SOCKET_SOCKET_H

#include "Typedefs.h"
#include "Log.h"
#include <winsock2.h>

#define DEFAULT_PORT (7171)
#define NO_SEND_FLAGS (0)
#define NO_RECV_FLAGS (0)
#define MILLISECONDS_IN_SECOND (1000)

/** Public socket functions **/
EResult Socket_OpenServerSocket(SOCKET* o_newSocket, SOCKET* o_newConnection);
EResult Socket_OpenClientSocket(SOCKET* o_newSocket);
EResult Socket_Send(SOCKET mySock, const char* buf, int len);
EResult Socket_Recv(SOCKET mySock, char* o_buf, int len, int* o_recv, int timeout);
EResult Socket_Cleanup();
/****************************/

/** Private socket functions **/
EResult socket_initializeWSA();
EResult socket_openSocket(SOCKET* o_newSocket);
/*****************************/

#endif //COMMON_COMMUNICATION_SOCKET_SOCKET_H