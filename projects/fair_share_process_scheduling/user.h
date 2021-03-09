#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>

#include "process.h"

class User
{
private:
    char id;
    std::vector<Process *> processes;

public:
    User(char);

    void addProcess(u_int32_t, u_int32_t);
    std::vector<Process *> getProcesses() { return processes; }
    //bool killProcess(Process *);
};

#endif
