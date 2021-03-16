#include "scheduler.hpp"

namespace switching
{
    scheduler::scheduler(size_t quantum): quantum(quantum)
    {
        this->create_thread();
    }

    scheduler::~scheduler()
    {
        this->terminate();
        for(user_t * user : this->users)
            delete user;
        for(process_t * process : this->processes)
            delete process;
    }

    user_t * scheduler::register_user(const std::string &name)
        {
            user_t * user = new user_t(name, this->quantum);
            this->users.push_back(user);
            return user;
        }

    process_t * scheduler::register_process(user_t * user, size_t arrival_time, size_t service_time)
    {
        
        size_t index = 0;
        if((index = find_user(user)) < 0)
            throw exceptions::invalid_user();

        this->pause();

        process_t * process = new process_t(this->users[index], arrival_time, service_time);
        processes.push_back(process);
        this->users[index]->increment_registered_processes();

        this->run();

        return process;
    }
    void scheduler::remove_process(process_t * process)
    {
        
        size_t index = 0;
        if((index = find_process(process)) < 0)
            throw exceptions::invalid_user();

        this->pause();
        
        this->processes[index]->get_user()->decrement_registered_processes();
        delete this->processes[index];
        this->processes.erase(this->processes.begin() + index);

        this->run();
    }

    int32_t scheduler::find_user(user_t * user)
    {
        for(int32_t i = 0; i < this->users.size(); i++)
            if(*this->users[i] == *user)
                return i;
        return -1;
    }
    int32_t scheduler::find_process(process_t * process)
    {
        for(int32_t i = 0; i < this->processes.size(); i++)
            if(*this->processes[i] == *process)
                return i;
        return -1;
    }

    const scheduler::users_t& scheduler::get_users() const { return this->users; }
    const scheduler::processes_t& scheduler::get_processes() const { return this->processes; }

    const size_t scheduler::get_quantum() const { return this->quantum; }
}
