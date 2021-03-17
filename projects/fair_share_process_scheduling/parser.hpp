#ifndef PARSER_HPP
#define PARSER_HPP

// #define DEBUG
// #define LOG_DEBUG (MSG) std::cout << "[DEBUG] " << MSG << "\n";

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

class Parser
{
public:
    //process related variables
    struct Process
    {
        uint32_t arrivalTime = 0;
        uint32_t serviceTime = 0;
    };
    //user related variables
    struct User
    {
        std::string name;
        std::vector<Process> processes;
        uint32_t processCount = 0;
    };
    //data obtained by parser
    struct Data
    {
        std::vector<User> users;
        uint32_t timeQuantum = 0;
    };

private:
    Data data;
    std::ifstream inputFile;
    std::string fileName;

public:
    //Constructor with input file path
    Parser(std::string FileName)
    {
        fileName = FileName;
    }

    //Parsing the input file assuming proper file structure
    void parse()
    {
        inputFile.open(fileName); //open the input file

        if (!inputFile.is_open())
        {
            throw "ERROR: Could not read from file";
        }

        std::string timeQuantumStr;
        getline(inputFile, timeQuantumStr); //get time quantum from first line

        data.timeQuantum = std::stoi(timeQuantumStr);

#ifdef DEBUG
        std::cout << "DEBUG Time Quantum: " << data.timeQuantum << std::endl;
#endif

        std::string line;
        std::string processCountStr;
        //Obtain 1 line at a time
        while (getline(inputFile, line))
        {
            User user;
            std::stringstream strStream(line); // first line of the loop is the user name and number of processes

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
            //loop through all processes information for 1 user
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

                user.processes.push_back(process); //add process to user

#ifdef DEBUG
                std::cout << "DEBUG       Process: " << process.arrivalTime << ", " << process.serviceTime << std::endl;
#endif

            }
            data.users.push_back(user); //add user to list of users
        }
        inputFile.close();
    }

    Data &getData()
    {
        return data;
    }
};

#endif
