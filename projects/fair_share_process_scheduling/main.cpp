#include <iostream>
#include <cstdlib>
#include <fstream>

#include "user.h"

std::ofstream out_file;
int main(int argc, char const *argv[])
{
    //Process CLI arguments, open file, check if successful
    std::string input_file = "input.txt";
    if (argc == 2)
        input_file = argv[1];
    std::ifstream file(input_file);

    if (!file.is_open())
    {
        std::cout << "ERROR: Could not read from file" << std::endl;
        return EXIT_FAILURE;
    }

    //Read file, store integers in vector, close file
    u_int32_t timeQuantum = 0;
    std::string line;
    std::vector<User *> userList;
    while (getline(file, line))
    {
        //User a('A');
        //User b('B');
        //std::vector<Process *> procList;

        // we assume that the input txt file has proper format
        switch (line.length())
        {
        case 1:
            timeQuantum = std::stoi(line);
            std::cout << "Time Quantum: " << timeQuantum << '\n';
            break;

        case 3:
            if(isalpha(line.at(1)))
            {
                userList.push_back(new User(line.at(1)));
            }
            else
            {
                userList.back()->addProcess(line.at(1), line.at(3));
            }

            break;
        default:
            std::cout << "There's a problem with input.txt format \n";
            break;
        }

        std::cout << line + '\n';
    }
    file.close();

    //Try to get the base path for the input (platform independent)
    std::string output_path = "output.txt";
    size_t slash_index = 0;
    if (
        ((slash_index = input_file.rfind('/')) != std::string::npos) ||
        ((slash_index = input_file.rfind('\\')) != std::string::npos))
        output_path = input_file.substr(0, slash_index + 1) + output_path;

    out_file = std::ofstream(output_path);

    return EXIT_SUCCESS;
}
