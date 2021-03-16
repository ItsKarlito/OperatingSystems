#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "scheduler.hpp"

class Parser
{
private:
    std::ifstream inputFile;

public:
    Parser(){}

    void openFile(std::string fileName)
    {
        inputFile.open(fileName);

        if (!inputFile.is_open())
        {
            throw "ERROR: Could not read from file";
        }
    }

    uint32_t getTimeQuantum()
    {
        uint32_t timeQuantum = 0;
        std::string timeQuantumStr;
        getline(inputFile, timeQuantumStr);

        timeQuantum = std::stoi(timeQuantumStr);
        std::cout << "Time Quantum: " << timeQuantum << '\n';

        return timeQuantum;
    }

    void parse(std::vector<switching::user_t> &userList, switching::scheduler &scheduler)
    {
        //Read file, store integers in vector, close file
        std::string line;
        std::string userName;
        std::string processCountStr;
        while (getline(inputFile, line))
        {
            std::stringstream strStream(line);

            strStream >> userName;
            switching::user_t * tempUser = scheduler.register_user(userName);
            userList.push_back(*tempUser);

            strStream >> processCountStr;
            u_int32_t processCount = std::stoi(processCountStr);
            userList.back().set_registered_processes(processCount);

            std::cout << "User Name: " << userName << '\n';
            std::cout << "  Process Count: " << processCount << '\n';

            for (int i = 0; i < processCount; i++) {
                getline(inputFile, line);

                strStream.str("");
                strStream.clear();
                strStream << line;

                std::string arrivalTimeStr;
                std::string serviceTimeStr;

                strStream >> arrivalTimeStr;
                strStream >> serviceTimeStr;

                scheduler.register_process(&userList.back(), std::stoi(arrivalTimeStr), std::stoi(serviceTimeStr));

                std::cout << "      Process: " << arrivalTimeStr << ", " << serviceTimeStr << std::endl;
            }
        }
    }
};

#endif
