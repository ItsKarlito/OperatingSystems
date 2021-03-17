#include <iostream>
#include <cstdlib>
#include <fstream>

#include "parser.hpp"
#include "writter.hpp"
#include "timer.hpp"

#include "scheduler.hpp"

int main(int argc, char const *argv[])
{
    std::string inputFileName = "input.txt";
    if (argc == 2)
    {
        inputFileName = (char *)argv[1];
    }

    Parser parser(inputFileName);

    try
    {
        parser.parse();
    }
    catch (const char *exception)
    {
        std::cout << exception << std::endl;
        return EXIT_FAILURE;
    }
 
    std::cout << std::endl << "Time Quantum: " << parser.getData().timeQuantum << '\n';

    for (int i = 0; i < parser.getData().users.size(); i++)
    {
        uint32_t processCount = parser.getData().users.at(i).processCount;
        std::cout << "User Name: " << parser.getData().users.at(i).name << '\n';
        std::cout << "  Process Count: " << parser.getData().users.at(i).processCount << '\n';
        for (int j = 0; j < processCount; j++) {
            std::cout << "      Process: " << parser.getData().users.at(i).processes.at(j).arrivalTime << ", " << parser.getData().users.at(i).processes.at(j).serviceTime << '\n';
        }
    }
    

    std::string output_path = "output.txt";
    size_t slash_index = 0;
    if (
        ((slash_index = inputFileName.rfind('/')) != std::string::npos) ||
        ((slash_index = inputFileName.rfind('\\')) != std::string::npos))
        output_path = inputFileName.substr(0, slash_index + 1) + output_path;

    return EXIT_SUCCESS;
}
