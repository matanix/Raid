#include "GeneralInfoRequest.h"
#include <Winbase.h>
#include <stdlib.h>
#include <Iphlpapi.h>

#pragma comment(lib, "IPHLPAPI.lib")

extern SOCKET g_sock;

EResult Messages_GeneralInfoRequest_Validate(char* payload, int payloadSize)
{
    if (payload != NULL || payloadSize != 0)
    {
        RAID_ERROR("GeneralInfoRequest should have no payload");
        return eResult_Failure;
    }

    return eResult_Success;
}

EResult Messages_GeneralInfoRequest_Execute(char* payload, int size)
{
    GeneralInfoResponse response;

    if (generalInfoRequest_getUserName(response.username) != eResult_Success)
    {
        RAID_ERROR("Failed to retrieve username");
        return eResult_Failure;
    }

    if (generalInfoRequest_getIP(response.ip) != eResult_Success)
    {
        RAID_ERROR("Failed to retreive IP");
        return eResult_Failure;
    }

    return RaidProtocol_SendMessage(g_sock, eRaidMessageType_GeneralInfoResponse, (char*)&response, sizeof(response));
}

EResult generalInfoRequest_getUserName(char* o_userName)
{
    int inOutSize = GENERAL_INFO_RESPONSE_MAX_USERNAME_SIZE;

    if (GetUserNameA(o_userName, &inOutSize) == 0)
    {
        RAID_ERROR("Failed to get username");
        return eResult_Failure;
    }

    return eResult_Success;
}

EResult generalInfoRequest_getIP(char* o_ip)
{
    //TODO:: ASSERT THAT THE WINDOWS IP LENGTH IS LIKE THE RESPONSE IP SIZE TO PREVENT OVERFLOW PREFERABLY USING STATIC ASSERT.

    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    int retVal = 0;
    int i;

    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof(IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL)
    {
        RAID_ERROR("Error allocating memory needed to call first GetAdaptersInfo");
        return eResult_Failure;
    }

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);
        if (pAdapterInfo == NULL) {
            RAID_ERROR("Error allocating memory needed to second GetAdaptersInfo");
            return eResult_Failure;
        }
    }
    RAID_INFO("Printing adapters info");

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR)
    {
        pAdapter = pAdapterInfo;
        while (pAdapter)
        {
            printf("\tComboIndex: \t%d\n", pAdapter->ComboIndex);
            printf("\tAdapter Name: \t%s\n", pAdapter->AdapterName);
            printf("\tAdapter Desc: \t%s\n", pAdapter->Description);
            printf("\tAdapter Addr: \t");

            for (i = 0; i < pAdapter->AddressLength; i++)
            {
                if (i == (pAdapter->AddressLength - 1))
                    printf("%.2X\n", (int) pAdapter->Address[i]);
                else
                    printf("%.2X-", (int) pAdapter->Address[i]);
            }

            memcpy(pAdapter->CurrentIpAddress->IpAddress.String, o_ip, sizeof(pAdapter->CurrentIpAddress->IpAddress.String));

            printf("\tIndex: \t%d\n", pAdapter->Index);
            printf("\tType: \t");
            switch (pAdapter->Type)
            {
                case MIB_IF_TYPE_OTHER:
                    printf("Other\n");
                    break;
                case MIB_IF_TYPE_ETHERNET:
                    printf("Ethernet\n");
                    break;
                case MIB_IF_TYPE_TOKENRING:
                    printf("Token Ring\n");
                    break;
                case MIB_IF_TYPE_FDDI:
                    printf("FDDI\n");
                    break;
                case MIB_IF_TYPE_PPP:
                    printf("PPP\n");
                    break;
                case MIB_IF_TYPE_LOOPBACK:
                    printf("Lookback\n");
                    break;
                case MIB_IF_TYPE_SLIP:
                    printf("Slip\n");
                    break;
                default:
                    printf("Unknown type %ld\n", pAdapter->Type);
                    break;
            }
        }
    }

    return eResult_Success;
}