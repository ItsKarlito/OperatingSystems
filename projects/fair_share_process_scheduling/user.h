#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>
#include <functional>

#include "process.hpp"

class User
{
private:
    char id;
    u_int32_t pid = 0; // tracking the pids. ONLY INCREMENT (never decrement) when adding new process to ensure pid uniqueness. (until we overflow of course)
    std::vector<Process *> processes;

public:
    User(char);
    ~User();

    void addProcess(const std::function<void()> &func);
    std::vector<Process *> getProcesses() { return processes; }
    
    // INCOMPLETE IMPLEMENTATION
    bool removeProcess(u_int32_t); 
};

#endif
