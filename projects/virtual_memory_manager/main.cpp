#include <iostream>
#include <cstdlib>

#include "writer.hpp"
#include "commandbuffer.hpp"
#include "scheduler.hpp"
#include "vmm.hpp"

int main(int argc, char const *argv[])
{
    srand(time(0));
    Writer writer;
    CommandBuffer cmdBuffer;
    std::string inputFilePath = "./projects/virtual_memory_manager/";
    if (argc == 2)
    {
        inputFilePath = (char *)argv[1];
    }

    Parser parser(inputFilePath);
    uint32_t numPages;

    try
    {
        numPages = parser.parseMemConfig();
        parser.parseProcess();
        parser.parseCommands();
        writer.initialize(inputFilePath);
    }
    catch (char const *e)
    {
        std::cout << e << '\n';
        return EXIT_FAILURE;
    }

    Parser::processData *pData = parser.getProcessData();
    Parser::cmdData *cData = parser.getCmdData();

    std::cout << "MEM DATA: \n  Number of pages: " << numPages << "\n\n";
    parser.printProcessData();
    cData->printCmdData();

    // // Writer test
    // try
    // {
    //     writer.write("MEM DATA: \n  Number of pages: " + std::to_string(numPages) + "\n\n");
    // }
    // catch (char const *e)
    // {
    //     std::cout << e << '\n';
    //     return EXIT_FAILURE;
    // }

    Timer<std::chrono::milliseconds> timer(1);
    timer.startTimer();

    vmm::vmm vmem_manager( numPages, "./vmem.bin", &cmdBuffer);
    vmem_manager.run();
    scheduler::Scheduler sched(cData, pData, &writer, &cmdBuffer, &timer);
    Writer* w = &writer;
    Timer<std::chrono::milliseconds> *t = &timer;
    vmem_manager.set_logger_callback(
        [w, t](std::string msg)
        {
            w->write("Clock: " + std::to_string(t->getElapsedTime()) + ", " + msg + "\n");
        }
    );
    vmem_manager.set_timer_callback(
        [t]()
        {
            return t->getElapsedTime();
        }
    );
    return EXIT_SUCCESS;
}
