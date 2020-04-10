#ifndef COMMON_LOG_H
#define COMMON_LOG_H

#include <stdio.h>
#define RAID_PRINT printf
#define RAID_PRINTLN(...) {RAID_PRINT(__VA_ARGS__); RAID_PRINT("\n");}
#define RAID_ERROR(...) {RAID_PRINT("ERROR: "); RAID_PRINTLN(__VA_ARGS__);}
#define RAID_INFO(...) {RAID_PRINT("INFO: "); RAID_PRINTLN(__VA_ARGS__);}

#endif //COMMON_LOG_H