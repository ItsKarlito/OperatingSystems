#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <chrono>
#include <thread>
#include <atomic>
#include <queue>
#include <algorithm>
#include <cstdlib>

#include "parser.hpp"
#include "writer.hpp"
#include "commandbuffer.hpp"
#include "timer.hpp"

namespace scheduler
{
    /******************GLOBALS******************/

    //Thread status definition
    enum status_t
    {
        RUNNING,
        IDLE,
        TERMINATED
    };

    /******************THREAD CONTROLLER******************/
    class thread_controller
    {
    protected:
        virtual void cycle() = 0; //to be executed when a thread is running, to be overwritten by each class implementation

        void create_thread(); //create thread function for processes

        std::thread &get_thread(); //return thread
        std::thread thread;
        std::mutex mtx; //mutex for status variable

    public:
        void set_status(status_t status); //change status of thread
        status_t get_status() const;      //check the status of current thread
        void run();                       //start thread
        void terminate();                 //terminate thread

    private:
        std::atomic<status_t> status;
        std::atomic<bool> alive;
    };

    /******************procT******************/
    //Process class that will execute commands
    class procT : public thread_controller
    {
    public:
        procT(size_t arrival_time, size_t service_time, uint32_t id, Writer *logger, CommandBuffer *cmdBuffer, Parser::cmdData *cData, Timer<std::chrono::milliseconds> *timer);
        ~procT();

        //getter and setter functions
        size_t get_service_time() const;
        size_t get_arrival_time() const;
        void set_service_time(size_t service_time);
        void set_start_end_time();
        uint32_t get_id() const;

        virtual void cycle() override; //override of the cycle function, aka what to execute when the process is running

    private:
        Parser::cmdData *commands;
        Writer *logger;
        CommandBuffer *cmdBuffer;
        Timer<std::chrono::milliseconds> *timer;
        std::atomic<size_t> service_time;
        std::atomic<size_t> arrival_time;
        std::atomic<size_t> start_time;
        std::atomic<size_t> end_time;
        uint32_t commandTime;
        uint32_t id;
    };

    /******************SCHEDULER******************/
    class Scheduler : public thread_controller
    {
    public:
        Scheduler(Parser::cmdData *cData, Parser::processData *pData, Writer *logger, CommandBuffer *cmdBuffer, Timer<std::chrono::milliseconds> *timer);
        ~Scheduler();

        void sortProcesses(); //sort all processes in order of arrival

        void myThread(); //scheduler thread function

        virtual void cycle() override; //what to execute when thread is live and running
    private:
        uint32_t numCores;
        uint32_t numProcess;
        std::queue<procT *> processes;
        std::vector<procT *> activeProcesses;

        Parser::cmdData *commands;
        Writer *logger;
        CommandBuffer *cmdBuffer;
        Timer<std::chrono::milliseconds> *timer;
        Parser::processData *pData;
    };
}

#endif