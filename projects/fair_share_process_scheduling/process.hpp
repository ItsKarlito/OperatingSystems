#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <atomic>
#include <functional>

class Process
{
private:
    static int base_id;
private:
    int id;
    std::function<void()> func;

    std::atomic<size_t> reserved_time;
public:
    Process(const std::function<void()> &func): 
        func(func), id(base_id++) {}
    
    const int getID() const {return id;}
    void changeReservedTime(int r) {int reserved_time = r;}
    const int getReservedTime() const {return reserved_time;}
};

#endif
