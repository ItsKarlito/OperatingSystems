#ifndef WRITER_HPP_
#define WRITER_HPP_

#include <mutex>
#include <iostream>
#include <fstream>

class Writer
{
private:
    std::ofstream outputFile;
    std::mutex logger_mutex;

public:
    Writer() {}

    // sets the output files destination
    void initialize(std::string outputFilePath)
    {
        if (outputFile.is_open()) throw "ERROR: Writer is already initialized";
        outputFile.open(outputFilePath + "output.txt");
        if (!outputFile.is_open()) throw "ERROR: Could not open output file";
    }

    void write(std::string string)
    {
        std::unique_lock<std::mutex> lck(logger_mutex);
        if (!outputFile.is_open()) throw "ERROR: Output file is closed";
        outputFile << string;
        std::cout << string;
    }

    ~Writer()
    {
        if(outputFile.is_open())
            outputFile.close();
    }
};

#endif
