#include "user.h"

User::User(char Id)
{
    id = Id;
}

void User::addProcess(u_int32_t ReadyTime, u_int32_t ServiceTime)
{
    processes.push_back(new Process(ReadyTime, ServiceTime));
};
