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
    while (getline(file, line))
    {
        User a('A');
        User b('B');
        std::vector<Process *> procList;

        // we assume that the input txt file has proper format
        switch (line.length())
        {
        case 1:
            timeQuantum = std::stoi(line);
            std::cout << "Time Quantum: " << timeQuantum << '\n';
            break;

        case 3:
            // TEST
            // a.addProcess(5, 3);
            // a.addProcess(1, 2);
            // a.addProcess(3, 6);
            // a.addProcess(2, 4);
            // procList = a.getProcesses();

            // for (int i = 0; i < procList.size(); i++)
            // {
            //     std::cout << "PID: " << procList.at(i)->getId() << '\n';
            // }

            // NOTE FOR THE BOISSS: process line to find user name (A,B,etc) and store user in data structure (vector located in main for example), find all users processes and readyTime, serviceTime for those processes and add them to the user using User::addProcess()
            break;
        default:
            // error in input.txt format
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
