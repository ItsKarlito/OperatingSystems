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
        commandBuffer_mtx.lock();
        if (commandBuffer.size() > 0)
        {
            commandBuffer.pop();
        }
        commandBuffer_mtx.unlock();
    }

    ~CommandBuffer() {}
};

#endif
