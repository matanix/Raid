#include <stdio.h>
#include "Communication/Socket/Socket.h"

#define MAIN_ERROR -1
#define MAIN_SUCCESS 0

int main(int argc, char* argv[])
{
    if (openSocket() != eResult_Success)
    {
        return MAIN_ERROR;
    }

    return MAIN_SUCCESS;
}
