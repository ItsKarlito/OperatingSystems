#include <iostream>
#include <cstdlib>
#include <fstream>

#include "parser.hpp"
#include "writter.hpp"
#include "timer.hpp"

int main(int argc, char const *argv[])
{
    std::vector<User> userList;
    Timer<std::chrono::seconds> timer(1);

    std::string inputFileName = "input.txt";
    if (argc == 2)
    {
        inputFileName = (char *)argv[1];
    }

    try
    {
        Parser parser(inputFileName);
        parser.parse(userList);
    }
    catch (const char *exception)
    {
        std::cout << exception << std::endl;
        return EXIT_FAILURE;
    }

    std::string output_path = "output.txt";
    size_t slash_index = 0;
    if (
        ((slash_index = inputFileName.rfind('/')) != std::string::npos) ||
        ((slash_index = inputFileName.rfind('\\')) != std::string::npos))
        output_path = inputFileName.substr(0, slash_index + 1) + output_path;

    Writter<std::chrono::seconds> writter(&timer);
    try
    {
        writter.openFile(output_path);
    }
    catch (const char *e)
    {
        std::cout << e << std::endl;
    }

    timer.startTimer();

    writter.fileOutput("B", 1, P_START);
    writter.fileOutput("A", 69, P_FINISH);

    //timer.stopTimer();

    return EXIT_SUCCESS;
}
