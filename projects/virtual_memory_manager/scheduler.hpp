#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <chrono>
#include <thread>
#include <atomic>

#include "parser.hpp"
#include "writer.hpp"

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
        static Parser::cmdData* commands;
        static Writer::Writer* logger;

        virtual void cycle() = 0;

        void create_thread();
        std::thread& get_thread();

    public:
        void set_status(status_t status);
        status_t get_status() const;
        void run();
        void terminate();

    private:
        
        std::thread thread;
        std::mutex mtx;
        std::atomic<status_t> status;
        std::atomic<bool> alive;
    };

    /******************PROCESS******************/
    class Process: public thread_controller
    {
    public:
        Process(size_t service_time, uint32_t id, Writer::Writer* logger);
        ~Process();

        size_t get_service_time() const;
        void set_service_time(size_t service_time);

        uint32_t get_id() const;

        virtual void cycle() override;

    private:
        Writer::Writer* logger;
        std::atomic<size_t> service_time;
        uint32_t id;
    };

    /******************SCHEDULER******************/
    class Scheduler: public thread_controller
    {
    public:
        Scheduler(Parser::cmdData* cData, Parser::processData* pData, Writer::Writer* logger);
        ~Scheduler();

        virtual void cycle() override;
    private:
        static Parser::processData* processes;
    };
}

#endif