#include "user.h"

User::User(std::string Id)
{
    id = Id;
}

void User::addProcess(u_int32_t ReadyTime, u_int32_t ServiceTime)
{
    processes.push_back(new Process(ReadyTime, ServiceTime, pid++));
};

bool User::removeProcess(u_int32_t Pid)
{
    bool exitStatus = 1;
    // locate process
    for (int i = 0; i < processes.size(); i++){
        if (processes.at(i)->getId() == Pid) {
            exitStatus = 0;
            // this should be called when the process serviceTime has been acheived and we can get rid of a process...
        }
    }
    return exitStatus;
};
