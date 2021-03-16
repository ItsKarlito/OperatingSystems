#ifndef WRITTER_HPP
#define WRITTER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "user.h"
#include "timer.hpp"

/*
    TODO: implement current time retrieval

    Class writter creates and output to "output.txt"
    To output an action call:
        fileOutput(std::string userName, int pID, output_action action)
    Arguments:
        userName - user's string name
        pID - process ID that the action relates to
        output_action - public enum
*/

enum output_action
{
    P_START,
    P_RESUME,
    P_PAUSE,
    P_FINISH
};

template<typename time_unit = std::chrono::seconds>
class Writter
{
private:
    std::ofstream outputFile;
    Timer<time_unit>* timer;

public:
    Writter(Timer<time_unit>* newTimer)
    {
        timer = newTimer;
        
    }

    void openFile(std::string outputFilePath)
    {
        outputFile.open(outputFilePath);

        if (!outputFile.is_open())
        {
            throw "ERROR: Could not open file";
        }
    }

    void fileOutput(std::string userName, int pID, output_action action)
    {
        if (!outputFile.is_open())
        {
            std::cout << "ERROR: Output file not opened\n";
            return;
        }

        int currentTime = timer->getElapsedTime();

        outputFile << "Time " << currentTime << ", User " << userName << ", Process " << pID;
        std::cout << "Time " << currentTime << ", User " << userName << ", Process " << pID;
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
