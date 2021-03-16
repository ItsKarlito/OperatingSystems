#ifndef WRITTER_HPP
#define WRITTER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "user.h"

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

class Writter
{
private:
    std::ofstream outputFile;
    int currentTime = 0;
    // TODO: create timer object

    int updateCurrentTime()
    {
        // TODO: implement getting time from timer class
        return 0;
    }

public:

    Writter()
    {
        
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
        //time = updateCurrentTime
        switch(action)
        {
            case P_START:
                outputFile << "User " << userName << ", Process " << pID << ", Started \n";
                break;
            case P_PAUSE:
                outputFile << "User " << userName << ", Process " << pID << ", Paused \n";
                break;
            case P_RESUME:
                outputFile << "User " << userName << ", Process " << pID << ", Resumed \n";
                break;
            case P_FINISH:
                outputFile << "User " << userName << ", Process " << pID << ", Finished \n";
                break;
            default:
                break;
        }
        
    }

};

#endif
