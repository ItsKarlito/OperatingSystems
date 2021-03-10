#include "user.h"

User::User(char Id)
{
    id = Id;
}
User::~User()
{
    for(Process* &p : this->processes)
    {
        delete p;
    }
}

void User::addProcess(const std::function<void()> &func)
{
    processes.push_back(new Process(func));
};

bool User::removeProcess(u_int32_t Pid)
{
    bool exitStatus = 1;
    // locate process
    for (int i = 0; i < processes.size(); i++){
        if (processes.at(i)->getID() == Pid) {
            exitStatus = 0;
            // this should be called when the process serviceTime has been acheived and we can get rid of a process...
        }
    }
    return exitStatus;
};
