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

EResult socket_openSocket(SOCKET* o_newSocket)
{
    if (socket_initializeWSA() != eResult_Success)
    {
        RAID_ERROR("Failed initialize WSA");
        return eResult_Failure;
    }

    *o_newSocket = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);

    if (*o_newSocket == INVALID_SOCKET)
    {
        RAID_ERROR("Could not create socket : %d" , WSAGetLastError());
        return eResult_Failure;
    }

    return eResult_Success;
}

EResult Socket_OpenClientSocket(SOCKET* o_newSocket)
{
    RAID_INFO("Opening socket");
    if (socket_openSocket(o_newSocket) != eResult_Success)
    {
        RAID_ERROR("Failed to open socket");
        return eResult_Failure;
    }

    SOCKADDR_IN serverAddr;
    int retCode = SOCKET_ERROR;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(DEFAULT_PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    RAID_INFO("Connecting socket");
    retCode = connect(*o_newSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (retCode != 0)
    {
        RAID_ERROR("Could not connect socket : %d" , WSAGetLastError());
        return eResult_Failure;
    }

    return eResult_Success;
}

EResult Socket_OpenServerSocket(SOCKET* o_newSocket, SOCKET* o_newConnection)
{
    SOCKADDR_IN serverAddr, clientInfo;
    int clientInfoLen = sizeof(clientInfo);

    RAID_INFO("Opening socket");
    if (socket_openSocket(o_newSocket) != eResult_Success)
    {
        RAID_ERROR("Failed to open socket");
        return eResult_Failure;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(DEFAULT_PORT);

    RAID_INFO("Binding socket");
    if (bind(*o_newSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) != 0)
    {
        RAID_ERROR("Socket bind failed : %d" , WSAGetLastError());
        return eResult_Failure;
    }

    RAID_INFO("Listening on socket");
    if ((listen(*o_newSocket, 1)) != 0)
    {
        RAID_ERROR("Socket listen failed");
        return eResult_Failure;
    }

    RAID_INFO("Accepting connection on socket");
    // Accept the data packet from client and verification
    *o_newConnection = accept(*o_newSocket, (SOCKADDR*)&clientInfo, &clientInfoLen);
    if (*o_newConnection == INVALID_SOCKET)
    {
        RAID_ERROR("Socket accept failed : %d" , WSAGetLastError());
        return eResult_Failure;
    }

    return eResult_Success;
}

EResult Socket_Send(SOCKET mySock, const char* buf, int len)
{
    if (mySock == INVALID_SOCKET)
    {
        RAID_ERROR("Invalid socket");
        return eResult_Failure;
    }

    if (buf == NULL)
    {
        RAID_ERROR("Null buff");
        return eResult_Failure;
    }

    if (send(mySock, buf, len, NO_SEND_FLAGS) < 0)
    {
        RAID_ERROR("Socket send failed: %d" , WSAGetLastError());
        return eResult_Failure;
    }

    return eResult_Success;
}

EResult Socket_Recv(SOCKET mySock, char* o_buf, int len, int* o_recv, int timeout)
{
    if (mySock == INVALID_SOCKET)
    {
        RAID_ERROR("Invalid socket.");
        return eResult_Failure;
    }

    if (o_buf == NULL)
    {
        RAID_ERROR("Null buff");
        return eResult_Failure;
    }

    if (o_recv == NULL)
    {
        RAID_ERROR("Null o_recv");
        return eResult_Failure;
    }

    DWORD actualTimeout = timeout * MILLISECONDS_IN_SECOND;
    setsockopt(mySock, SOL_SOCKET, SO_RCVTIMEO, (char*)&actualTimeout, sizeof(actualTimeout));

    *o_recv = recv(mySock, o_buf, len, NO_RECV_FLAGS);
    if (*o_recv == SOCKET_ERROR)
    {
        *o_recv = 0;

        if (WSAGetLastError() == WSAETIMEDOUT)
        {
            return eResult_Timeout;
        }

        RAID_ERROR("Socket error failed with : %d" , WSAGetLastError());
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
            RAID_ERROR("Failed to cleanup WSA : %d" , WSAGetLastError());
            return eResult_Failure;
        }
    }

    return eResult_Success;
}