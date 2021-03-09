#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>

#include "process.h"

class User
{
private:
    char id;
    u_int32_t pid = 0; // tracking the pids. ONLY INCREMENT (never decrement) when adding new process to ensure pid uniqueness. (until we overflow of course)
    std::vector<Process *> processes;

public:
    User(char);

    void addProcess(u_int32_t, u_int32_t);
    std::vector<Process *> getProcesses() { return processes; }
    
    // INCOMPLETE IMPLEMENTATION
    bool removeProcess(u_int32_t); 
};

#endif
