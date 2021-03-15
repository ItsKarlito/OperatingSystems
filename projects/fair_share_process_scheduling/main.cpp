#include <iostream>
#include <cstdlib>
#include <fstream>

#include "parser.hpp"
#include "writter.hpp"

int main(int argc, char const *argv[])
{
    std::vector<User> userList;
    
    std::string fileName = "input.txt";
    if (argc == 2)
    {
        fileName = argv[1];
    }

    try
    {
        Parser parser(fileName);
        parser.parse(userList);
    }
    catch (const char *exception)
    {
        std::cout << exception << std::endl;
        return EXIT_FAILURE;
    }

    Writter writter;
    try
    {
        writter.openFile();
    }
    catch(const char *e)
    {
        std::cout << e << std::endl;
    }

    return EXIT_SUCCESS;
}
