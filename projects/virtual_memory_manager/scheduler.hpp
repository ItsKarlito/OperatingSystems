#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <chrono>
#include <thread>
#include <atomic>

#include "parser.hpp"

/******************GLOBALS******************/

typedef std::chrono::milliseconds time_unit_t;

enum status_t
{
    RUNNING,
    IDLE,
    TERMINATED
};

Parser::cmdData* commands;
Parser::processData* processes;

/******************THREAD CONTROLLER******************/
class thread_controller
{
protected:
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
    Process(size_t service_time, uint32_t id);
    ~Process();

    size_t get_service_time() const;
    void set_service_time(size_t service_time);

    uint32_t get_id() const;

    virtual void cycle() override;

private:
    std::atomic<size_t> service_time;
    uint32_t id;
};

/******************SCHEDULER******************/
class Scheduler: public thread_controller
{

    Scheduler()
    {

    }
    ~Scheduler()
    {
        
    }
};

#endif