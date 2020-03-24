#ifndef COMMON_TYPEDEFS_H
#define COMMON_TYPEDEFS_H

#include <stdio.h>

#define WAIT_FOR_KEY_PRESS() getchar()
#define NULL_TERMINATOR_SIZE (1)
#define NULL_TERMINATOR ('\0')

typedef enum { false = 0, true = 1} bool;

typedef enum
{
    eResult_Success = 0,
    eResult_Failure
} EResult;

#endif //COMMON_TYPEDEFS_H