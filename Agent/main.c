#include <stdio.h>
#include "Communication/Socket/Socket.h"
#include "Communication/RaidProtocol/RaidProtocol.h"
#include "RaidAgent/RaidAgent.h"

#define MAIN_ERROR -1
#define MAIN_SUCCESS 0

SOCKET g_sock = INVALID_SOCKET;

static EResult initializeAgent()
{
    if (Socket_OpenClientSocket(&g_sock) != eResult_Success)
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
    RAID_INFO("Initializing agent");
    if (initializeAgent() != eResult_Success)
    {
        RAID_ERROR("Failed to initialize agent");
        goto main_err;
    }
    RAID_INFO("Agent initialized");

    RAID_INFO("Checking RaidProtocol sanity vs server");
    if (RaidProtocol_ClientSanity(g_sock) != eResult_Success)
    {
        RAID_ERROR("Communication sanity failed");
        goto main_err;
    }
    RAID_INFO("---------Sanity success-------------");

    if (RaidAgent_Run() != eResult_Success)
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
