#ifndef COMMON_LOG_H
#define COMMON_LOG_H

#include <stdio.h>
#define RAID_PRINT printf
#define RAID_ERROR(...) {RAID_PRINT("ERROR: "); RAID_PRINT(__VA_ARGS__); RAID_PRINT("\n");}
#define RAID_INFO(...) {RAID_PRINT("INFO: "); RAID_PRINT(__VA_ARGS__); RAID_PRINT("\n");}

#endif //COMMON_LOG_H