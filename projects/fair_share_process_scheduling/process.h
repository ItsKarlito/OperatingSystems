#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>

class Process
{
private:
    u_int32_t id;
    u_int32_t readyTime;
    u_int32_t serviceTime;

public:
    Process(u_int32_t ReadyTime, u_int32_t ServiceTime,  u_int32_t id) {}

    u_int32_t getReadyTime() { return readyTime; }
    u_int32_t getServiceTime() { return serviceTime; }
    u_int32_t getId() { return id; }
};

#endif
