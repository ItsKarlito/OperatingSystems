#ifndef WRITER_HPP_
#define WRITER_HPP_

#include <mutex>
#include <iostream>
#include <fstream>
#include <exception>
#include "timer.hpp"

class Writer
{
private:
    std::ofstream outputFile;
    std::mutex logger_mutex;

    Timer<std::chrono::milliseconds> *timer;

public:
    // Constructor. Initializes the writer. Will
    // throw an exception if path can't be oppened
    Writer(std::string path, Timer<std::chrono::milliseconds> *timer) : timer(timer)
    {
        outputFile.open(path);
        if (!this->outputFile.is_open())
            throw std::runtime_error("ERROR: Could not open output file");
    }

    // Writes string in both log file and cout
    void write(std::string string)
    {
        std::unique_lock<std::mutex> lck(logger_mutex);
        string =
            std::string("Clock: ") + std::to_string(timer->getElapsedTime()) + ", " + string + "\n";
        std::cout << string;
        if (this->outputFile.is_open())
        {
            this->outputFile << string;
            this->outputFile.flush();
        }
    }

    // Destructor. Closes files
    ~Writer()
    {
        if (outputFile.is_open())
            outputFile.close();
    }

    // Get current time from timer
    uint64_t getElapsedTime()
    {
        return this->timer->getElapsedTime();
    }
};

#endif
