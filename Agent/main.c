#include <stdio.h>
#include "Communication/Socket/Socket.h"

#define MAIN_ERROR -1
#define MAIN_SUCCESS 0

static EResult initializeAgent()
{
    SOCKET sock = INVALID_SOCKET;
    if (Socket_OpenSocket(&sock) != eResult_Success)
    {
        RAID_ERROR("Failed to open socket");
        goto error_cleanup;
    }

    if (Socket_Cleanup() != eResult_Success)
    {
        RAID_ERROR("Socket_Cleanup failed");
        return eResult_Failure;
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
        return MAIN_ERROR;
    }
    RAID_INFO("Agent initialized");

    return MAIN_SUCCESS;
}
