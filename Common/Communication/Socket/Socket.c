#include "Socket.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

static bool isInitialized = false;

EResult socket_initializeWSA()
{
    static WSADATA wsaData;

    if (isInitialized)
    {
        return eResult_Success;
    }

    RAID_INFO("Initialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        RAID_ERROR("Failed. Error Code : %d", WSAGetLastError());
        return eResult_Failure;
    }
    RAID_INFO("Initialised Winsock.");

    isInitialized = true;
    return eResult_Success;
}

EResult Socket_OpenSocket(SOCKET* o_newSocket)
{
    SOCKADDR_IN serverAddr;
    int retCode = SOCKET_ERROR;

    if (socket_initializeWSA() != eResult_Success)
    {
        return eResult_Failure;
    }

    if ((*o_newSocket = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP )) == INVALID_SOCKET)
    {
        RAID_ERROR("Could not create socket : %d" , WSAGetLastError());
        return eResult_Failure;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(DEFAULT_PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retCode = connect(*o_newSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (retCode != 0)
    {
        RAID_ERROR("Could not connect socket : %d" , WSAGetLastError());
        return eResult_Failure;
    }

    return eResult_Success;
}

EResult Socket_Cleanup()
{
    if (isInitialized)
    {
        if (WSACleanup() != 0)
        {
            RAID_ERROR("Failed to cleanup WSA");
            return eResult_Failure;
        }
    }

    return eResult_Success;
}