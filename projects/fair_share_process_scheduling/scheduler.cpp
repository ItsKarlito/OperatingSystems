#include "scheduler.hpp"

namespace switching
{
    scheduler::scheduler(size_t quantum, logger_t logger): quantum(quantum), logger(logger), current_process(0)
    {
        this->create_thread();
    }
    scheduler::scheduler(size_t quantum): scheduler::scheduler(quantum, nullptr) {}
    

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
        int32_t index = 0;
        if((index = find_user(user)) < 0)
            throw exceptions::invalid_user();

        process_t * process = new process_t(this->users[index], arrival_time, service_time);
        processes.push_back(process);
        this->users[index]->increment_registered_processes();

        this->update_quantums();

        return process;
    }
    void scheduler::remove_process(process_t * process)
    {
        int32_t index = 0;
        if((index = find_process(process)) < 0)
            throw exceptions::invalid_user();
        
        this->processes[index]->get_user()->decrement_registered_processes();
        delete this->processes[index];
        this->processes.erase(this->processes.begin() + index);

        this->update_quantums();
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

    void scheduler::update_quantums()
    {
        size_t pc = this->processes.size();
        if(pc == 0)
            return;

        for(user_t *u : this->users)
        {
            size_t rp = u->get_registered_processes();
            if(rp < 1)
                continue;
            size_t new_quantum = this->quantum * ((float)rp/(float)pc);
            u->update_quantum(new_quantum < 1 ? 1 : new_quantum);
        }
    }

    void scheduler::cycle()
    {
        std::unique_lock<std::mutex> lck(this->done_mtx);
        size_t process_count = this->processes.size();
        this->done = process_count == 0;
        done_cv.notify_all();
        if(this->done)
        {
            this->pause();
            return;
        }
        lck.unlock();

        this->current_process = this->current_process >= process_count ? 0 : this->current_process;

        process_t* cp = this->processes[this->current_process];
        std::string base_log = "Process " + std::to_string(cp->get_id()) + " from user " + cp->get_user()->get_name() + " ";

        this->log(base_log + "now running (" + std::to_string(cp->get_user()->get_burst()) + "s burst)");
        cp->run();
        std::this_thread::sleep_for(time_unit_t(cp->get_user()->get_burst()));

        this->log(base_log + "has been paused");
        cp->pause();
        bool should_increment = true;
        if(cp->get_service_time() == 0)
        {
            this->remove_process(cp);
            this->log(base_log + "has been terminated");
            should_increment = false;
        }
        
        this->current_process++;
    }

    void scheduler::log(const std::string &msg) const
    {
        if(this->logger != nullptr)
            this->logger(msg);
    }

    void scheduler::wait_for_done()
    {
        std::unique_lock<std::mutex> lck(this->done_mtx);
        while(this->done == false) done_cv.wait(lck);
    }
}
