#include "process.h"

Process::Process(u_int32_t ReadyTime, u_int32_t ServiceTime, u_int32_t Id)
{
    id = Id;
    readyTime = ReadyTime;
    serviceTime = ServiceTime;
}
