#include "scheduler.hpp"

namespace scheduler
{
    /******************THREAD CONTROLLER******************/
    void thread_controller::create_thread()
    {

    }

    std::thread& thread_controller::get_thread()
    {
        return this->thread;
    }

    void thread_controller::set_status(status_t status)
    {
        this->mtx.lock();
        this->status = status;
        this->mtx.unlock();
    }

    status_t thread_controller::get_status() const
    {
        return this->status;
    }
    void thread_controller::run()
    {
        this->set_status(status_t::RUNNING);
    }
    void thread_controller::terminate()
    {
        this->set_status(status_t::TERMINATED);
        if(this->thread.joinable())
            this->thread.join();
    }

    /******************PROCESS******************/
    Process::Process(size_t service_time, uint32_t id, Writer::Writer* logger)
    {
        this->service_time = service_time;
        this->id = id;
        this->logger = logger;
        this->create_thread();
    }
    Process::~Process()
    {
        this->terminate();
    }

    size_t Process::get_service_time() const
    {
        return this->service_time;
    }
    void Process::set_service_time(size_t service_time)
    {
        this->service_time = service_time;
    }

    uint32_t Process::get_id() const
    {
        return this->id;
    }

    void Process::cycle()
    {

    }


    /******************SCHEDULER******************/
    Scheduler::Scheduler(Parser::cmdData* commandData, Parser::processData* pData, Writer::Writer* logger)
    {
        this->commands = commandData;
        this->processes = pData;
        this->logger = logger;
        this->create_thread();
    }
    Scheduler::~Scheduler()
    {
        this->terminate();
        this->commands->deleteCmd();
        this->processes->processes.clear();
    }
    void Scheduler::cycle()
    {

    }
}