#include <stdio.h>
#include "Communication/Socket/Socket.h"
#include "Communication/RaidProtocol/RaidProtocol.h"
#include "RaidStation/RaidStation.h"

#define MAIN_ERROR -1
#define MAIN_SUCCESS 0

SOCKET g_sock = INVALID_SOCKET;
SOCKET g_conn = INVALID_SOCKET;

static EResult initializeStation()
{
    if (Socket_OpenServerSocket(&g_sock, &g_conn, RAID_PORT) != eResult_Success)
    {
        RAID_ERROR("Failed to open socket");
        goto error_cleanup;
    }

    return eResult_Success;

    error_cleanup:
        if (Socket_Cleanup() != eResult_Success)
        {
            RAID_ERROR("Socket_Cleanup failed");
        }

        return eResult_Failure;
}


int main(int argc, char* argv[])
{
    RAID_INFO("Initializing station");
    if (initializeStation() != eResult_Success)
    {
        RAID_ERROR("Failed to initialize station");
        goto main_err;
    }
    RAID_INFO("Station initialized");

    RAID_INFO("Checking RaidProtocol sanity vs client");
    if (RaidProtocol_ServerSanity(g_conn) != eResult_Success)
    {
        RAID_ERROR("Communication sanity failed");
        goto main_err;
    }
    RAID_INFO("---------Sanity success-------------");

    if (RaidStation_Init() != eResult_Success)
    {
        RAID_ERROR("Failed to initialize RaidAgent");
        goto main_err;
    }

    if (RaidStation_Run() != eResult_Success)
    {
        RAID_ERROR("Failed to run RaidAgent");
        goto main_err;
    }


    WAIT_FOR_KEY_PRESS();
    return MAIN_SUCCESS;

    main_err:
        WAIT_FOR_KEY_PRESS();
        return MAIN_ERROR;
}
