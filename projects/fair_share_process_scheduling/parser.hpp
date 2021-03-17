#ifndef PARSER_HPP
#define PARSER_HPP

//#define DEBUG

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

class Parser
{
private:
    struct Process
    {
        uint32_t arrivalTime;
        uint32_t serviceTime;
    };

    struct User
    {
        std::string name;
        std::vector<Process> processes;
        uint32_t processCount;
    };

    struct Data
    {
        std::vector<User> users;
        uint32_t timeQuantum;
    };

    std::ifstream inputFile;
    std::string fileName;
    Data data;

public:
    Parser(std::string FileName)
    {
        fileName = FileName;
    }

    void parse()
    {
        inputFile.open(fileName);

        if (!inputFile.is_open())
        {
            throw "ERROR: Could not read from file";
        }

        //Read file, store integers in vector, close file
        std::string timeQuantumStr;
        getline(inputFile, timeQuantumStr);

        data.timeQuantum = std::stoi(timeQuantumStr);

#ifdef DEBUG
        std::cout << "Time Quantum: " << data.timeQuantum << std::endl;
#endif

        std::string line;
        std::string processCountStr;
        while (getline(inputFile, line))
        {
            User user;
            std::stringstream strStream(line);

            strStream >> user.name;
            data.users.push_back(user);

            strStream >> processCountStr;
            user.processCount = std::stoi(processCountStr);

#ifdef DEBUG
            std::cout << "User Name: " << user.name << std::endl;
            std::cout << "  Process Count: " << user.processCount << std::endl;
#endif

            for (int i = 0; i < user.processCount; i++)
            {
                getline(inputFile, line);

                strStream.str("");
                strStream.clear();
                strStream << line;

                Process process;
                std::string arrivalTimeStr;
                std::string serviceTimeStr;

                strStream >> arrivalTimeStr;
                strStream >> serviceTimeStr;

                process.arrivalTime = std::stoi(arrivalTimeStr);
                process.serviceTime = std::stoi(serviceTimeStr);

                user.processes.push_back(process);

#ifdef DEBUG
                std::cout << "      Process: " << process.arrivalTime << ", " << process.serviceTime << std::endl;
#endif

            }
        }
        inputFile.close();
    }

    Data &getData()
    {
        return data;
    }
};

#endif
