#include "switching.hpp"

namespace switching
{
    //*******************************************************************************
    //*********************************USER HANDLING*********************************
    //*******************************************************************************
    
    uint32_t user_t::id_counter = 0;
    
    user_t::user_t(const std::string &name, size_t quantum): 
    name(name), id(id_counter++), burst(0), registered_processes(0), quantum(quantum < 1 ? 1 : quantum) {}

    uint32_t user_t::get_id() const { return id; }

    size_t user_t::get_burst() const { return this->burst; }
    void user_t::update_burst() 
    {
        if(this->registered_processes == 0)
            return;
        this->burst = this->quantum/this->registered_processes;
        this->burst = this->burst < 1 ? 1: this->burst;
    }

    void user_t::increment_registered_processes() 
    {
        this->set_registered_processes(this->registered_processes+1);
    }
    void user_t::decrement_registered_processes() 
    {
        this->set_registered_processes(this->registered_processes > 0 ? this->registered_processes-- : 0);
    }

    size_t user_t::get_registered_processes() const { return this->registered_processes;}
    void user_t::set_registered_processes(size_t registered_processes) 
    {
        this->registered_processes = registered_processes;
        this->update_burst();
    }

    void user_t::update_quantum(size_t quantum)
    {
        this->quantum = quantum == 0 ? 1 : quantum;
        this->update_burst();
    }

    const std::string& user_t::get_name() const { return name; }

    //*******************************************************************************
    //********************************THREAD HANDLING********************************
    //*******************************************************************************
    
    void thread_controller::create_thread()
    {
        if(this->thread.joinable())
            return;
        
        this->alive = true;
        this->status = status_t::IDLE;
        this->thread = std::thread([&](){
            while(this->alive)
            {
                // Wait until the condition variable has been set to not
                // IDLE
                std::unique_lock<std::mutex> lck(this->mtx);
                while(this->status == status_t::IDLE) cv.wait(lck);
                lck.unlock();

                switch (this->status)
                {
                case status_t::IDLE:
                    break;
                case status_t::TERMINATED:
                    this->alive = false;
                    break;                
                case status_t::RUNNING:
                    this->cycle();
                    break;
                }
            }
            this->status = status_t::TERMINATED;
        });
    }
    std::thread& thread_controller::get_thread() { return this->thread; }

    status_t thread_controller::get_status() const { return this->status; }
    void thread_controller::set_status(status_t status) 
    {
        std::unique_lock<std::mutex> lck(this->mtx);
        this->status = status;
        cv.notify_all();
    }

    void thread_controller::run() { this->set_status(status_t::RUNNING); }
    void thread_controller::pause() { this->set_status(status_t::IDLE); }
    void thread_controller::terminate() 
    { 
        this->set_status(status_t::TERMINATED); 
        if(this->thread.joinable())
            this->thread.join(); 
    }

    //********************************************************************************
    //********************************PROCESS HANDLING********************************
    //********************************************************************************
    
    process_t::process_t(user_t* user, size_t arrival_time, size_t service_time):
        user(user), arrival_time(arrival_time), service_time(service_time), virgin(true)
    {
        if(this->user == nullptr)
            throw exceptions::null_pointer_error();
        
        this->create_thread();
        this->id = user->increment_process_id();
    }
    process_t::~process_t()
    {
        this->terminate();
    }

    size_t process_t::get_arrival_time() const { return this->arrival_time; }
    size_t process_t::get_service_time() const { return this->service_time; }
    void process_t::set_service_time(size_t service_time) {this->service_time = service_time; }

    uint32_t process_t::get_id() const { return this->id; }

    user_t * process_t::get_user() { return this->user; }

    void process_t::cycle()
    {
        // Just count down your service time, pause when done
        if(this->service_time > 0)
        {
            this->service_time--;
            std::this_thread::sleep_for(time_unit_t(1));
        }
        else 
            this->pause();
    }

    void process_t::set_virgin(bool v)
    {
        this->virgin = v;
    }
    bool process_t::is_virgin()
    {
        return this->virgin;
    }

    //*******************************************************************************
    //*******************************SCHEDULE HANDLING*******************************
    //*******************************************************************************
    
    scheduler::scheduler(size_t quantum, Writer::writerFunctor_t logger): quantum(quantum), logger(logger), current_process(0), done(false)
    {
        this->create_thread();
    }
    scheduler::scheduler(size_t quantum): scheduler::scheduler(quantum, nullptr) {}
    

    scheduler::~scheduler()
    {
        // Make sure no threads nor memory spaces are left unchecked
        this->terminate();
        for(user_t * user : this->users)
            delete user;
        for(process_t * process : this->processes)
            delete process;
    }

    user_t * scheduler::register_user(const std::string &name)
        {
            user_t * user = new user_t(name, this->quantum);
            std::unique_lock<std::mutex> lck(scheduling_mutex);
            this->users.push_back(user);
            return user;
        }

    process_t * scheduler::register_process(user_t * user, size_t arrival_time, size_t service_time)
    {
        int32_t index = 0;

        // Make sure the user already exists before registering a process
        if((index = find_user(user)) < 0)
            throw exceptions::invalid_user();

        std::unique_lock<std::mutex> lck(scheduling_mutex);
        process_t * process = new process_t(this->users[index], arrival_time, service_time);
        this->processes.push_back(process);

        // Update your users processes and quantums.
        this->users[index]->increment_registered_processes();
        this->update_quantums();

        return process;
    }
    void scheduler::remove_process(process_t * process)
    {
        int32_t index = 0;

        //Make sure the process already exists before deleting it
        if((index = find_process(process)) < 0)
            throw exceptions::invalid_user();
        
        std::unique_lock<std::mutex> lck(scheduling_mutex);
        this->processes[index]->get_user()->decrement_registered_processes();
        delete this->processes[index];
        this->processes.erase(this->processes.begin() + index);

        this->update_quantums();
    }

    int32_t scheduler::find_user(user_t * user)
    {
        std::unique_lock<std::mutex> lck(scheduling_mutex);
        for(int32_t i = 0; i < this->users.size(); i++)
            if(*this->users[i] == *user)
                return i;
        return -1;
    }
    int32_t scheduler::find_process(process_t * process)
    {
        std::unique_lock<std::mutex> lck(scheduling_mutex);
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

        // Only count the users that are currently
        // doing jobs for the quantum calculation
        size_t active_users = 0;
        for(user_t *u : this->users)
        {
            if(u->get_registered_processes() > 0)
                active_users++;
        }

        size_t new_quantum = this->quantum/(float)active_users;
        new_quantum = new_quantum < 1 ? 1 : new_quantum;

        for(user_t *u : this->users)
        {
            size_t rp = u->get_registered_processes();

            // If the user has no processes, no point in updating it
            if(rp < 1)
                continue;

            u->update_quantum(new_quantum < 1 ? 1 : new_quantum);
        }
    }

    void scheduler::cycle()
    {
        std::unique_lock<std::mutex> lck(this->done_mtx);
        std::unique_lock<std::mutex> s_lck(scheduling_mutex);
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

        cp->run();

        //Check if process has been run in the past, if so, send a start action
        if(cp->is_virgin())
        {
            this->log(cp->get_user()->get_name(), cp->get_id(), Writer::output_action::P_START);
            cp->set_virgin(false);
        }
        this->log(cp->get_user()->get_name(), cp->get_id(), Writer::output_action::P_RESUME);

        //Wait out the process burst
        std::this_thread::sleep_for(time_unit_t(cp->get_user()->get_burst() > cp->get_service_time() ? cp->get_service_time() : cp->get_user()->get_burst()));

        cp->pause();
        s_lck.unlock();

        this->log(cp->get_user()->get_name(), cp->get_id(), Writer::output_action::P_PAUSE);

        //If the process is done, remove it
        if(cp->get_service_time() == 0)
        {
            this->remove_process(cp);
            this->log(cp->get_user()->get_name(), cp->get_id(), Writer::output_action::P_FINISH);
        }
        
        this->current_process++;
    }

    void scheduler::log(std::string userName, int pID, Writer::output_action action)
    {
        if(this->logger != nullptr)
            this->logger(userName, pID, action);
    }

    void scheduler::wait_for_done()
    {
        std::unique_lock<std::mutex> lck(this->done_mtx);
        while(this->done == false) done_cv.wait(lck);
    }
}