#include <iostream>
#include <cstdlib>

#include "parser.hpp"

int main(int argc, char const *argv[])
{
    std::string inputFilePath = "/projects/fair_share_process_scheduling/";
    if(argc == 2)
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
    }
    catch(char const* e)
    {
        std::cout << "Error: " << e << '\n';
    }

    Parser::processData *pData = parser.getProcessData();
    Parser::cmdData *cData = parser.getCmdData();

    parser.printProcessData();
    cData->printCmdData();

    return EXIT_SUCCESS;
}
