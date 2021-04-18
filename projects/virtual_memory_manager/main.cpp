#include <iostream>
#include <cstdlib>
#include <functional>

#include "writer.hpp"
#include "commandbuffer.hpp"
#include "scheduler.hpp"
#include "vmm.hpp"

int main(int argc, char const *argv[])
{
    Parser* parser = nullptr;
    Writer* writer = nullptr;
    Timer<std::chrono::milliseconds>* timer = nullptr;
    vmm::vmm_thread* vmem_manager = nullptr;
    CommandBuffer* cmdBuffer = nullptr;
    size_t number_of_pages = 0;

    // Lambda used to deinitialize all of the
    // object pointers used for the scheduler
    std::function<void()> deinitialize(
        [=]()
        {
            if(parser != nullptr)
                delete parser;
            if(writer != nullptr)
                delete writer;
            if(timer != nullptr)
                delete timer;
            if(vmem_manager != nullptr)
                delete vmem_manager;
            if(cmdBuffer != nullptr)
                delete cmdBuffer;
        }
    );

    // Initialize random number generator
    srand(time(0));

    // Get root file path
    std::string root_file_path = "./projects/virtual_memory_manager/config/";
    if (argc == 2)
        root_file_path = (char *)argv[1];

    try
    {
        // Initialize utilities
        parser = new Parser(root_file_path);
        timer = new Timer<std::chrono::milliseconds>(1);
        timer->startTimer();
        writer = new Writer(root_file_path + "../output.txt", timer);
        cmdBuffer = new CommandBuffer();

        // Parse from config file
        number_of_pages = parser->parseMemConfig();
        parser->parseProcess();
        parser->parseCommands();

        // Initialze and start virtual memory
        // manager
        vmem_manager = new vmm::vmm_thread(
            number_of_pages,
            root_file_path + "../vmem.txt", 
            cmdBuffer,
            timer, 
            writer
        );
        vmem_manager->run();

        // Get parser data
        Parser::processData *pData = parser->getProcessData();
        Parser::cmdData *cData = parser->getCmdData();
        std::cout << "MEM DATA: \n  Number of pages: " << number_of_pages << "\n\n";
        parser->printProcessData();
        cData->printCmdData();

        // Start scheduler
        scheduler::Scheduler sched(cData, pData, writer, cmdBuffer, timer);
    }
    catch(const std::exception& e)
    {
        deinitialize();
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    deinitialize();
    return EXIT_SUCCESS;
}
