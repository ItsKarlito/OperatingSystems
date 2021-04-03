#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <vector>
#include <mutex>
#include <sstream>

class Parser
{
    public:
    enum command_type {STORE, LOOKUP, RELEASE, ERROR};

    struct Process
    {
        uint32_t arrivalTime;
        uint32_t serviceTime;
    };

    struct Command
    {
        command_type name;
        uint32_t id;
        uint32_t value;
    };

    struct processData
    {
        std::vector<Process> processes;
        uint32_t numCores;
        uint32_t numProcess;
    };

    struct cmdData
    {
        Command getCommand()
        {
            return commands.at(currentCmd++);
        }

        void addCmd(Command cmd)
        {
            commands.push_back(cmd);
        }

        private:
        std::vector<Command> commands;
        uint32_t currentCmd;
        std::mutex cmdMutex;
    };

    Parser(std::string filePath) : inputPath(filePath),
                                    memconfigFileName("memconfig.txt"),
                                    processFileName("process.txt"),
                                    commandsFileName("commands.txt") {}

    uint32_t parseMemConfig()
    {
        std::string file = inputPath + memconfigFileName;
        memconfigFile.open(file);
        if (!memconfigFile.is_open())
        {
            throw "ERROR: Could not read from file";
        }

        std::string numPages;
        getline(memconfigFile,numPages);
        return std::stoi(numPages);
        
    }

    void parseProcess()
    {
        std::string file = inputPath + processFileName;
        processFile.open(file);
        if (!processFile.is_open())
        {
            throw "ERROR: Could not read from file";
        }

        std::string line;
        getline(processFile,line);
        pData.numCores = std::stoi(line);

        getline(processFile,line);
        pData.numProcess = std::stoi(line);

        std::stringstream strStream;
        while(getline(processFile,line))
        {
            Process proc;
            strStream.str("");
            strStream.clear();
            strStream << line;

            std::string tempVal;
            strStream >> tempVal;
            proc.arrivalTime = std::stoi(tempVal);
            strStream >> tempVal;
            proc.serviceTime = std::stoi(tempVal);

            pData.processes.push_back(proc);
        }
    }

    void parseCommands()
    {
        std::string file = inputPath + commandsFileName;
        commandsFile.open(file);
        if (!commandsFile.is_open())
        {
            throw "ERROR: Could not read from file";
        }

        std::string line;
        std::stringstream strStream;

        while(getline(commandsFile,line))
        {
            Command cmd;
            int arguments;
            command_type currentCommand;
            strStream.str("");
            strStream.clear();
            strStream << line;

            std::string tempVal;
            strStream >> tempVal;
            currentCommand = commandConvert(tempVal);
            cmd.name = currentCommand;

            if(currentCommand == STORE)
            {
                strStream >> tempVal;
                cmd.id = std::stoi(tempVal);
                strStream >> tempVal;
                cmd.value = std::stoi(tempVal);
            }
            else if(currentCommand == RELEASE || currentCommand == LOOKUP)
            {
                strStream >> tempVal;
                cmd.id = std::stoi(tempVal);
                cmd.value = 0;
            }

            
            
        }
    }

    ~Parser() {}

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

    command_type commandConvert(std::string const& cmd)
    {
        if(cmd == "Store") return STORE;
        else if(cmd == "Lookup") return LOOKUP;
        else if(cmd == "Release") return RELEASE;
        else return ERROR;
    }

};

#endif