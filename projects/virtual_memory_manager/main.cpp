#include <iostream>
#include <cstdlib>

#include "writer.hpp"
#include "parser.hpp"

int main(int argc, char const *argv[])
{
    Writer writer;
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

    // Writer test
    try
    {
        writer.write("MEM DATA: \n  Number of pages: " + std::to_string(numPages) + "\n\n");
    }
    catch (char const *e)
    {
        std::cout << e << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
