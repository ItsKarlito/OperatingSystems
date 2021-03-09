#include "process.h"

Process::Process(u_int32_t ReadyTime, u_int32_t ServiceTime)
{
    id = uniqueId++;
    readyTime = ReadyTime;
    serviceTime = ServiceTime;
}
