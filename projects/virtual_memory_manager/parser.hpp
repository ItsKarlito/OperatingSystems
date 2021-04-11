#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <vector>
#include <mutex>
#include <sstream>
#include <iostream>

#define PARSER_DEBUG

class Parser
{
public:
    enum command_type
    {
        STORE,
        LOOKUP,
        RELEASE,
        ERROR
    };

    struct Process
    {
        uint32_t id;
        uint32_t arrivalTime;
        uint32_t serviceTime;

        bool operator < (const Process& p) const
        {
            return(arrivalTime < p.arrivalTime);
        }
    };

    struct Command
    {
        command_type name;
        uint32_t id;
        int value;

        void printCommand()
        {
            if(name == STORE) std::cout << "Store: ";
            else if(name == LOOKUP) std::cout << "Lookup: ";
            else std::cout << "Release: ";

            std::cout << id << ", " << value << std::endl;
        }
    };

    struct processData
    {
        std::vector<Process> processes;
        int numCores;
        int numProcess;
    };

    struct cmdData
    {
        Command getCommand()
        {
            if(currentCmd < (commands.size() - 1))
            {
                return commands.at(currentCmd++);
            }
            else if(currentCmd == (commands.size() - 1))
            {
                uint32_t tempCurrent = currentCmd;
                currentCmd = 0;
                return commands.at(tempCurrent);
            }
            else
            {
                currentCmd = 0;
                return commands.at(currentCmd);
            }
        }

        void addCmd(Command cmd)
        {
            commands.push_back(cmd);
        }

        void deleteCmd()
        {
            commands.clear();
        }

#ifdef PARSER_DEBUG
        void printCmdData()
        {
            std::cout << "COMMAND DATA: \n";
            std::cout << "  Current command: " << currentCmd << std::endl;
            for (int i = 0; i < commands.size(); i++)
            {
                std::cout << "  Command " << i << ": ";
                switch (commands.at(i).name)
                {
                case STORE:
                    std::cout << "Store, " << commands.at(i).id << ", " << commands.at(i).value << std::endl;
                    break;
                case RELEASE:
                    std::cout << "Release, " << commands.at(i).id << std::endl;
                    break;
                case LOOKUP:
                    std::cout << "Lookup, " << commands.at(i).id << std::endl;
                    break;
                default:
                    break;
                }
            }
            std::cout << std::endl;
        }
#endif

    private:
        std::vector<Command> commands;
        uint32_t currentCmd;
        std::mutex cmdMutex;
    };

    Parser(std::string filePath) : inputPath(filePath),
                                   memconfigFileName("memconfig.txt"),
                                   processFileName("processes.txt"),
                                   commandsFileName("commands.txt") {}

    processData *getProcessData() { return &pData; }

    cmdData *getCmdData() { return &cData; }

    uint32_t parseMemConfig()
    {
        std::string file = inputPath + memconfigFileName;
        memconfigFile.open(file);
        if (!memconfigFile.is_open())
        {
            throw "ERROR: Could not read from memconfig file";
        }

        std::string numPages;
        getline(memconfigFile, numPages);
        return std::stoi(numPages);
    }

    void parseProcess()
    {
        std::string file = inputPath + processFileName;
        processFile.open(file);
        if (!processFile.is_open())
        {
            throw "ERROR: Could not read from process file";
        }

        std::string line;
        getline(processFile, line);
        pData.numCores = std::stoi(line);

        if(pData.numCores < 1)
        {
            throw "ERROR: Number of cores must be at least 1";
        }

        getline(processFile, line);
        pData.numProcess = std::stoi(line);

        if(pData.numProcess < 0)
        {
            throw "ERROR: Number of processes cannot be negative";
        }

        std::stringstream strStream;
        for(int i = 0; i < pData.numProcess; i++)
        {
            if(!processFile.eof())
            {
                getline(processFile, line);
                Process proc;
                strStream.str("");
                strStream.clear();
                strStream << line;

                std::string tempVal;
                strStream >> tempVal;
                proc.arrivalTime = std::stoi(tempVal);
                strStream >> tempVal;
                proc.serviceTime = std::stoi(tempVal);

                proc.id = i+1;

                pData.processes.push_back(proc);
            }
            else
            {
                throw "ERROR: The number of processes provided is bigger than the number of arrival/service time pairs";
            }
        }
        if(!processFile.eof())
        {
            throw "ERROR: The number of processes provided is smaller than the number of arrival/service time pairs";
        }
    }

    void parseCommands()
    {
        std::string file = inputPath + commandsFileName;
        commandsFile.open(file);
        if (!commandsFile.is_open())
        {
            throw "ERROR: Could not read from commands file";
        }

        std::string line;
        std::stringstream strStream;

        while (getline(commandsFile, line))
        {
            Command cmd;
            command_type currentCommand;
            strStream.str("");
            strStream.clear();
            strStream << line;

            std::string tempVal;
            strStream >> tempVal;
            currentCommand = commandConvert(tempVal);

            if (currentCommand == STORE)
            {
                cmd.name = currentCommand;
                strStream >> tempVal;
                cmd.id = std::stoi(tempVal);
                strStream >> tempVal;
                cmd.value = std::stoi(tempVal);

                cData.addCmd(cmd);
            }
            else if (currentCommand == RELEASE || currentCommand == LOOKUP)
            {
                cmd.name = currentCommand;
                strStream >> tempVal;
                cmd.id = std::stoi(tempVal);
                cmd.value = 0;

                cData.addCmd(cmd);
            }
        }
    }
#ifdef PARSER_DEBUG
    void printProcessData()
    {
        std::cout << "PROCESS DATA:\n";
        std::cout << "  Number of cores: " << pData.numCores << std::endl;
        std::cout << "  Number of processes: " << pData.numProcess << std::endl;
        for (int i = 0; i < pData.processes.size(); i++)
        {
            std::cout << "  Process " << i << " arrival and service times: " << pData.processes.at(i).arrivalTime << ", " << pData.processes.at(i).serviceTime << std::endl;
        }
        std::cout << std::endl;
    }
#endif

    ~Parser()
    {
    }

private:
    std::ifstream memconfigFile;
    std::ifstream processFile;
    std::ifstream commandsFile;

    std::string inputPath;
    std::string memconfigFileName;
    std::string processFileName;
    std::string commandsFileName;

    processData pData = {};
    cmdData cData = {};

    command_type commandConvert(std::string cmd)
    {
        if (cmd == "Store")
            return STORE;
        else if (cmd == "Lookup")
            return LOOKUP;
        else if (cmd == "Release")
            return RELEASE;
        else
            return ERROR;
    }
};

#endif
