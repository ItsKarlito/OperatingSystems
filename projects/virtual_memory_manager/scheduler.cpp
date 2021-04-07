#include "scheduler.hpp"

namespace scheduler
{
    /******************THREAD CONTROLLER******************/
    void thread_controller::create_thread()
    {
        if(this->thread.joinable())
            return;

        this->alive = true;
        this->status = status_t::IDLE;
        this->thread = std::thread([&](){
            while(this->alive)
            {
                status_t current_status = this->get_status();

                switch(current_status)
                {
                    case status_t::IDLE:
                        break;
                    case status_t::TERMINATED:
                        this->alive = false;
                        break;
                    case status_t::RUNNING:
                        this->cycle();
                        break;
                    default:
                        break;
                    
                }
            }
            this->set_status(status_t::TERMINATED);
        });
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
    Process::Process(size_t service_time, uint32_t id, Writer* logger, Parser::cmdData* cData, Timer<std::chrono::milliseconds>* timer)
    {
        this->service_time = service_time*1000;
        this->id = id;
        this->commandTime = 0;
        this->commands = cData;
        this->logger = logger;
        this->timer = timer;
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
        this->service_time = service_time*1000;
    }

    void Process::set_start_end_time()
    {
        this->start_time = this->timer->getElapsedTime();
        this->end_time = this->start_time + this->service_time;
    }

    uint32_t Process::get_id() const
    {
        return this->id;
    }

    void Process::cycle()
    {
        uint64_t currentTime = this->timer->getElapsedTime();
        if(currentTime < this->end_time)
        {
            if(this->commandTime == 0)
            {
                uint32_t toWait = rand() % (this->end_time - currentTime);
                this->commandTime = toWait + currentTime;
            }
            else if(this->commandTime <= currentTime)
            {
                std::cout << currentTime << ": Execute next command\n";
            }
        }
        else
        {
            this->terminate();
        }
    }


    /******************SCHEDULER******************/
    Scheduler::Scheduler(Parser::cmdData* commandData, Parser::processData* pData, Writer* logger, Timer<std::chrono::milliseconds>* timer)
    {
        this->commands = commandData;
        this->pData = pData;
        this->logger = logger;
        this->timer = timer;
        this->create_thread();
    }

    Scheduler::~Scheduler()
    {
        this->terminate();
        this->commands->deleteCmd();
        this->pData->processes.clear();
    }

    void Scheduler::sortProcesses()
    {
        std::sort(pData->processes.begin(), pData->processes.end());
    }

    void Scheduler::cycle()
    {
        
    }
}