#ifndef COMMANDBUFFER_HPP_
#define COMMANDBUFFER_HPP_

#include <queue>
#include <mutex>
#include "parser.hpp"

class CommandBuffer
{
private:
    std::mutex commandBuffer_mtx;
    std::queue<Parser::Command> commandBuffer;

public:
    CommandBuffer() {}

    void pushCmd(Parser::Command cmd)
    {
        commandBuffer_mtx.lock();
        commandBuffer.push(cmd);
        commandBuffer_mtx.unlock();
    }

    Parser::Command popCmd()
    {
        Parser::Command cmd;
        commandBuffer_mtx.lock();
        if (commandBuffer.size() > 0)
        {
            cmd = commandBuffer.front(); // get cmd in front of queue
            commandBuffer.pop();         // delete cmd in front of queue
        }
        commandBuffer_mtx.unlock();
        return cmd;
    }

    std::size_t getSize()
    {
        std::unique_lock<std::mutex> lck(commandBuffer_mtx);
        return commandBuffer.size();
    }

    ~CommandBuffer() {}
};

#endif
