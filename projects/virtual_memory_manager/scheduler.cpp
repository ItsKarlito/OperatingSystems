#include "scheduler.hpp"

namespace scheduler
{
    /******************THREAD CONTROLLER******************/
    void thread_controller::create_thread()
    {
        std::cout << "create thread\n";
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

    /******************procT******************/
    procT::procT(size_t service_time, uint32_t id, Writer* logger, Parser::cmdData* cData, Timer<std::chrono::milliseconds>* timer)
    {
        this->service_time = service_time*1000;
        this->id = id;
        this->commandTime = 0;
        this->commands = cData;
        this->logger = logger;
        this->timer = timer;
        this->create_thread();
    }
    procT::~procT()
    {
        this->terminate();
    }

    size_t procT::get_service_time() const
    {
        return this->service_time;
    }

    size_t procT::get_arrival_time() const
    {
        return this->arrival_time;
    }

    void procT::set_service_time(size_t service_time)
    {
        this->service_time = service_time*1000;
    }

    void procT::set_start_end_time()
    {
        this->start_time = this->timer->getElapsedTime();
        this->end_time = this->start_time + this->service_time;
        std::cout << "Process " << this->id << " starts at " << this->start_time << std::endl;
    }

    uint32_t procT::get_id() const
    {
        return this->id;
    }

    void procT::cycle()
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
                Parser::Command cmd = commands->getCommand();
                std::cout << currentTime << ", Execute: ";
                cmd.printCommand();
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
        this->numCores = pData->numCores;
        this->numProcess = pData->numProcess;
        this->sortProcesses();
        std::cout << "about to create thread\n";
        this->thread = std::thread(&Scheduler::myThread, this);
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
        for(int i = 0; i < pData->processes.size(); i++)
        {
            Parser::Process tempProc = pData->processes.at(i);
            procT * tempThread = new procT(tempProc.serviceTime, tempProc.id, this->logger, this->commands, this->timer);
            this->processes.push(tempThread);
        }
        std::cout << "*Scheduler* Processes sorted" << std::endl;
    }

    void Scheduler::myThread()
    {
        bool alive = true;
        this->set_status(status_t::RUNNING);
        while(alive)
        {
            status_t current_status = this->get_status();

            switch(current_status)
            {
                case status_t::IDLE:
                    break;
                case status_t::TERMINATED:
                    alive = false;
                    break;
                case status_t::RUNNING:

                    this->cycle();
                    break;
                default:
                    break;
                
            }
        }
        this->set_status(status_t::TERMINATED);
    }

    void Scheduler::cycle()
    {
        uint64_t currentTime = timer->getElapsedTime();
        if(currentTime%1000 == 0)
        {
            std::cout << "second " << currentTime << std::endl;
            for(int i = 0; i < this->activeProcesses.size(); i++)
            {
                if(this->activeProcesses.at(i)->get_status() == status_t::TERMINATED)
                {
                    numCores++;
                    this->activeProcesses.at(i) = this->activeProcesses.back();
                    this->activeProcesses.pop_back();
                    i--;
                }
            }
            while(this->numCores > 0)
            {
                if(!this->processes.empty())
                {
                    if(this->processes.front()->get_arrival_time() <= currentTime)
                    {
                        this->processes.front()->set_start_end_time();
                        this->processes.front()->run();
                        this->activeProcesses.push_back(this->processes.front());
                        this->processes.pop();
                        numCores--;
                    }
                    else{
                        break;
                    }
                }
                else
                {
                    std::cout << "this->processes is empty\n";
                    break;
                }
            }
        }
    }
}