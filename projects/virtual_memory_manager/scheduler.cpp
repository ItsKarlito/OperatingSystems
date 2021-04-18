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
        });
    }

    std::thread& thread_controller::get_thread()    {return this->thread;}

    status_t thread_controller::get_status() const  {return this->status;}

    void thread_controller::run()   {this->set_status(status_t::RUNNING);}

    void thread_controller::set_status(status_t status)
    {
        this->mtx.lock();
        this->status = status;
        this->mtx.unlock();
    }

    void thread_controller::terminate()
    {
        this->set_status(status_t::TERMINATED);
        if(this->thread.joinable())
            this->thread.join();
    }

    /******************procT******************/
    procT::procT(size_t arrival_time, size_t service_time, uint32_t id, Writer* logger, CommandBuffer* cmdBuffer, Parser::cmdData* cData, Timer<std::chrono::milliseconds>* timer)
    {
        this->arrival_time = arrival_time*1000; //translate from seconds to milliseconds
        this->service_time = service_time*1000; //translate from seconds to milliseconds
        this->id = id;
        this->commandTime = 0;
        this->commands = cData;
        this->logger = logger;
        this->cmdBuffer = cmdBuffer;
        this->timer = timer;
        this->create_thread();
    }
    procT::~procT()
    {
        this->terminate();
    }

    size_t procT::get_service_time() const  {return this->service_time;}

    size_t procT::get_arrival_time() const  {return this->arrival_time;}

    void procT::set_service_time(size_t service_time)   {this->service_time = service_time*1000;}

    uint32_t procT::get_id() const  {return this->id;}

    //When the process is meant to start, se the start time (not necessarily arrival time) and finish time
    void procT::set_start_end_time()
    {
        this->start_time = this->timer->getElapsedTime();
        this->end_time = this->start_time + this->service_time;
        logger->write("Clock: " + std::to_string(this->start_time) + ", Process " + std::to_string(this->id) + ": Started\n");
    }

    void procT::cycle()
    {
        uint64_t currentTime = this->timer->getElapsedTime(); //get current time
        if(currentTime < this->end_time) //check if process is finished
        {
            if(this->commandTime == 0) //if there is no command execution
            {
                uint32_t toWait = rand() % 1000 + 1; //get random wait time between 1 and 1000ms
                this->commandTime = toWait + currentTime;   //set when the command is meant to be executed
                this->commandTime = std::min(this->commandTime, (uint32_t)this->end_time);  //select minimum between random command execution time and end of process
            }
            else if(this->commandTime <= currentTime)   //command is ready to be executed
            {
                Parser::Command cmd = commands->getCommand();   //get next command
                cmdBuffer->pushCmd(cmd);
                logger->write("Clock: " + std::to_string(currentTime) + ", Process " + std::to_string(this->id) + ", " + cmd.printCommand() + '\n'); //print command that will be executed
                this->commandTime = 0;  //reset command wait time
            }
        }
        else
        {
            //terminate process
            logger->write("Clock: " + std::to_string(currentTime) + ", Process " + std::to_string(this->id) + ": Finished\n");
            this->set_status(status_t::TERMINATED);
        }
    }


    /******************SCHEDULER******************/
    Scheduler::Scheduler(Parser::cmdData* commandData, Parser::processData* pData, Writer* logger, CommandBuffer* cmdBuffer, Timer<std::chrono::milliseconds>* timer)
    {
        this->commands = commandData;
        this->pData = pData;
        this->logger = logger;
        this->cmdBuffer = cmdBuffer;
        this->timer = timer;
        this->numCores = pData->numCores;
        this->numProcess = pData->numProcess;
        this->sortProcesses();
        this->thread = std::thread(&Scheduler::myThread, this);
    }

    Scheduler::~Scheduler()
    {
        this->terminate();
        this->commands->deleteCmd();
        this->pData->processes.clear();
    }

    //Sorts processes in the order of arrival and populates the procT queue
    void Scheduler::sortProcesses()
    {
        std::sort(pData->processes.begin(), pData->processes.end());
        for(int i = 0; i < pData->processes.size(); i++)
        {
            Parser::Process tempProc = pData->processes.at(i);
            procT * tempThread = new procT(tempProc.arrivalTime, tempProc.serviceTime, tempProc.id, this->logger, this->cmdBuffer, this->commands, this->timer);
            this->processes.push(tempThread);
        }
    }

    //Thread of the scheduler, by default it will be running
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

    //What to execute by the Scheduler
    void Scheduler::cycle()
    {
        uint64_t currentTime = timer->getElapsedTime(); //get current time
        if(currentTime%1000 == 0) //We will take action upon each second
        {
            //check the vector of active processes, if a process terminated then remove that process and free up 1 core per terminated process
            for(int i = 0; i < this->activeProcesses.size(); i++)
            {
                if(this->activeProcesses.at(i)->get_status() == status_t::TERMINATED)
                {
                    this->activeProcesses.at(i)->terminate();
                    numCores++;
                    this->activeProcesses.at(i) = this->activeProcesses.back();
                    this->activeProcesses.pop_back();
                    i--;
                }
            }
            while(this->numCores > 0)   //while there are still available cores
            {
                if(!this->processes.empty())    //check if there are still processes to be executed
                {
                    if(this->processes.front()->get_arrival_time() <= currentTime)  //check the arrival time of the next process
                    {
                        this->processes.front()->set_start_end_time();  //set start and finish time of the process to be run
                        this->processes.front()->run(); //run the process
                        this->activeProcesses.push_back(this->processes.front()); //add process to active process vector
                        this->processes.pop();  //remove process from wait queue
                        numCores--; //decrement number of available cores
                    }
                    else{
                        break;  //no process to be executed for now
                    }
                }
                else
                {
                    break;  //no more processes to be executed
                }
            }
            if(this->activeProcesses.empty() && this->processes.empty()) //no more processes waiting, and no more active process. Terminate scheduler
            {
                this->set_status(status_t::TERMINATED);
                //logger->write("Clock: " + std::to_string(currentTime) + ", Stopping Execution\n");
                this->timer->stopTimer();
            }
        }
    }
}