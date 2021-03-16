#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "user.h"

class Parser
{
private:
    std::ifstream inputFile;

public:
    Parser(std::string fileName)
    {
        inputFile.open(fileName);

        if (!inputFile.is_open())
        {
            throw "ERROR: Could not read from file";
        }
    }

    void parse(std::vector<User> &userList, u_int32_t &timeQuantum)
    {
        //Read file, store integers in vector, close file
        std::string timeQuantumStr;
        getline(inputFile, timeQuantumStr);

        timeQuantum = std::stoi(timeQuantumStr);
        std::cout << "Time Quantum: " << timeQuantum << '\n';

        std::string line;
        std::string userName;
        std::string processCountStr;
        while (getline(inputFile, line))
        {
            std::stringstream strStream(line);

            strStream >> userName;
            User tempUser(userName);
            userList.push_back(tempUser);

            strStream >> processCountStr;
            u_int32_t processCount = std::stoi(processCountStr);

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

                tempUser.addProcess(std::stoi(arrivalTimeStr), std::stoi(serviceTimeStr));

                std::cout << "      Process " << tempUser.getProcesses().back()->getId() << ": " << tempUser.getProcesses().back()->getReadyTime() << ", " << tempUser.getProcesses().back()->getServiceTime() << '\n';
            }
        }
    }
};

#endif
