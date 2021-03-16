#ifndef SWITCHING_HPP_
#define SWITCHING_HPP_

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <ctime>
#include <cstdint>
#include <vector>
#include <atomic>
#include <exception>
#include <chrono>

#define PROCESS_DEBUG

namespace switching 
{
    typedef std::chrono::seconds time_unit_t;

    enum status_t
    {
        RUNNING,
        IDLE,
        TERMINATED
    };

    class exceptions
    {
    public:
        struct null_pointer_error : public std::exception
        {const char * what () const throw () {return "null referenced passed to object";}};
        struct invalid_user : public std::exception
        {const char * what () const throw () {return "specified user has not been registered";}};
        struct invalid_process : public std::exception
        {const char * what () const throw () {return "specified process has not been registered";}};

    };
}

#endif