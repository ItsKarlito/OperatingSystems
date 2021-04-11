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
#include "timer.hpp"

namespace scheduler
{
    /******************GLOBALS******************/

    typedef std::chrono::milliseconds time_unit_t;

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
        virtual void cycle() = 0;

        void create_thread();
        std::thread& get_thread();

        std::thread thread;

    public:
        void set_status(status_t status);
        status_t get_status() const;
        void run();
        void terminate();

    private:
        
        //std::thread thread;
        std::mutex mtx;
        std::atomic<status_t> status;
        std::atomic<bool> alive;
    };

    /******************procT******************/
    class procT: public thread_controller
    {
    public:
        procT(size_t arrival_time, size_t service_time, uint32_t id, Writer* logger, Parser::cmdData* cData, Timer<std::chrono::milliseconds>* timer);
        ~procT();

        size_t get_service_time() const;
        size_t get_arrival_time() const;
        void set_service_time(size_t service_time);
        void set_start_end_time();

        uint32_t get_id() const;

        virtual void cycle() override;

    private:
        Parser::cmdData* commands;
        Writer* logger;
        Timer<std::chrono::milliseconds>* timer;
        std::atomic<size_t> service_time;
        std::atomic<size_t> arrival_time;
        std::atomic<size_t> start_time;
        std::atomic<size_t> end_time;
        uint32_t commandTime;
        uint32_t id;
    };

    /******************SCHEDULER******************/
    class Scheduler: public thread_controller
    {
    public:
        Scheduler(Parser::cmdData* cData, Parser::processData* pData, Writer* logger, Timer<std::chrono::milliseconds>* timer);
        ~Scheduler();

        void sortProcesses();

        void myThread();

        virtual void cycle() override;
    private:
        uint32_t numCores;
        uint32_t numProcess;
        std::queue<procT*> processes;
        std::vector<procT*> activeProcesses;
        
        Parser::cmdData* commands;
        Writer* logger;
        Timer<std::chrono::milliseconds>* timer;
        Parser::processData* pData;
    };
}

#endif