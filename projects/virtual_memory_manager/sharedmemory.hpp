#ifndef SHAREDMEMORY_HPP_
#define SHAREDMEMORY_HPP_

#include <queue>
#include <mutex>
#include "parser.hpp"

class SharedMemory
{
private:
    std::mutex commands_mtx;
    std::queue<Parser::Command> commands;

public:
    SharedMemory() {}

    void pushCmd(Parser::Command cmd)
    {
        commands_mtx.lock();
        commands.push(cmd);
        commands_mtx.unlock();
    }

    Parser::Command popCmd()
    {
        commands_mtx.lock();
        if (commands.size() > 0)
        {
            commands.pop();
        }
        commands_mtx.unlock();
    }

    ~SharedMemory() {}
};

#endif
