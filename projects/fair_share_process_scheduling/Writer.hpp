#ifndef WRITER_HPP_
#define WRITER_HPP_

#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include <sstream>
#include <ctime>

#include "timer.hpp"

/*
    Class writter creates and output to "output.txt"
    To output an action call:
        fileOutput(std::string userName, int pID, output_action action)
    Arguments:
        userName - user's string name
        pID - process ID that the action relates to
        output_action - public enum
*/

class Writer
{
public:
    //actions that a process can do that we will output to file
    enum output_action
    {
        P_START,
        P_RESUME,
        P_PAUSE,
        P_FINISH
    };

    //function call to output to file
    typedef std::function<void(std::string userName, int pID, output_action action)> writerFunctor_t;

private:
    std::ofstream outputFile;
    size_t offset = 0;

private:
    //getting the current time, required by writer class itself
    size_t get_current_time()
    {
        return time(NULL);
    }

public:
    Writer() 
    {
        this->set_offset(0);
    }
    ~Writer()
    {
        if(outputFile.is_open())
            outputFile.close();
    }

    //offset required so that execution time starts at time 1 and not 0
    void set_offset()
    {
        this->offset = this->get_current_time();
    }
    void set_offset(size_t t)
    { 
        this->offset = t;
    }

    void openFile(std::string outputFilePath)
    {
        outputFile.open(outputFilePath);

        if (!outputFile.is_open())
        {
            throw "ERROR: Could not open file";
        }
    }

    //function call user by scheduler to output to file
    void fileOutput(std::string userName, int pID, output_action action)
    {
        if (!outputFile.is_open())
        {
            std::cout << "ERROR: Output file not opened\n";
            return;
        }

        long int currentTime = this->get_current_time() - this->offset +1; //get current time at which the function is called

        outputFile << "Time " << currentTime << ", User " << userName << ", Process " << pID; //output to file time, user, and process information
        std::cout << "Time " << currentTime << ", User " << userName << ", Process " << pID;
        //to output the action that was performed, check the action required
        switch(action)
        {
            case P_START:
                outputFile << ", Started \n";
                std::cout << ", Started \n";
                break;
            case P_PAUSE:
                outputFile << ", Paused \n";
                std::cout << ", Paused \n";
                break;
            case P_RESUME:
                outputFile << ", Resumed \n";
                std::cout << ", Resumed \n";
                break;
            case P_FINISH:
                outputFile << ", Finished \n";
                std::cout << ", Finished \n";
                break;
            default:
                break;
        }
        
    }

};

#endif
