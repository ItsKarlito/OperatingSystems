#ifndef PARSER_HPP
#define PARSER_HPP

#define DEBUG
#define LOG_DEBUG (MSG) std::cout << "[DEBUG] " << MSG << "\n";

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

class Parser
{
private:
    struct Process
    {
        uint32_t arrivalTime = 0;
        uint32_t serviceTime = 0;
    };

    struct User
    {
        std::string name;
        std::vector<Process> processes;
        uint32_t processCount = 0;
    };

    struct Data
    {
        std::vector<User> users;
        uint32_t timeQuantum = 0;
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
        std::cout << "DEBUG Time Quantum: " << data.timeQuantum << std::endl;
#endif

        std::string line;
        std::string processCountStr;
        while (getline(inputFile, line))
        {
            User user;
            std::stringstream strStream(line);

            strStream >> user.name;

            strStream >> processCountStr;
#ifdef DEBUG
            std::cout << "DEBUG Process Count String: " << std::stoi(processCountStr) << std::endl;
#endif
            user.processCount = std::stoi(processCountStr);

#ifdef DEBUG
            std::cout << "DEBUG User Name: " << user.name << std::endl;
            std::cout << "DEBUG   Process Count: " << user.processCount << std::endl;
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
                std::cout << "DEBUG       Process: " << process.arrivalTime << ", " << process.serviceTime << std::endl;
#endif

            }
            data.users.push_back(user);
        }
        inputFile.close();
    }

    Data &getData()
    {
        return data;
    }
};

#endif
