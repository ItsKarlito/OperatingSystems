#ifndef SWITCHING_HPP_
#define SWITCHING_HPP_

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <ctime>
#include <cstdint>
#include <vector>
#include <atomic>
#include <exception>
#include <chrono>

namespace switching 
{
    typedef std::chrono::seconds time_unit_t;

    enum status_t
    {
        RUNNING,
        IDLE,
        TERMINATED
    };

    class exceptions
    {
    public:
        struct null_pointer_error : public std::exception
        {const char * what () const throw () {return "null referenced passed to object";}};
        struct invalid_user : public std::exception
        {const char * what () const throw () {return "specified user has not been registered";}};
        struct invalid_process : public std::exception
        {const char * what () const throw () {return "specified process has not been registered";}};

    };
    class user_t
    {
    public:
        user_t(const std::string &name, size_t quantum): name(name), id(id_counter++), burst(0), registered_processes(0), quantum(quantum){}

        uint32_t get_id() const {return id;} 

        bool operator==(const user_t& user) const {return this->id == user.get_id();} 

        size_t get_burst() const {return this->burst;}
        void update_burst() 
        {
            this->burst = this->quantum/this->registered_processes;
            this->burst = this->burst < 0 ? 1: this->burst;
        }

        void increment_registered_processes() 
        {
            this->set_registered_processes(this->registered_processes++);
        }
        void decrement_registered_processes() 
        {
            this->set_registered_processes(this->registered_processes > 0 ? this->registered_processes-- : 0);
        }

        size_t get_registered_processes() const {return this->registered_processes;}
        void set_registered_processes(size_t registered_processes) 
        {
            this->registered_processes = registered_processes;
            this->update_burst();
        }

        const std::string& get_name() const {return name;}

    private:
        size_t burst;
        const size_t quantum;
        size_t registered_processes;
        const uint32_t id;
        static uint32_t id_counter;
        std::string name;
    };
    
    class process_t 
    {
    public:
        process_t(const user_t* user, size_t arrival_time, size_t service_time);
        ~process_t();

        std::thread& get_thread() {return this->thread;}

        status_t get_status() const {return this->status;}
        void set_status(status_t status) 
        {
            std::unique_lock<std::mutex> lck(this->mtx);
            this->status = status;
            cv.notify_all();
        }

        void run() {this->set_status(status_t::RUNNING);}
        void pause() {this->set_status(status_t::IDLE);}
        void terminate() {this->set_status(status_t::TERMINATED); this->thread.join();}

        size_t get_arrival_time() const {return this->arrival_time;}
        size_t get_service_time() const {return this->service_time;}
        void set_service_time(size_t service_time) {this->service_time = service_time;}

        uint32_t get_id() const {return this->id;}

        bool operator==(const process_t& process) const {return this->id == process.get_id();}

        user_t * get_user() {return this->user;}

    private:
        void create_thread();

    private:
        user_t* user;
        std::atomic<size_t> service_time;
        const size_t arrival_time;

        std::thread thread;
        std::mutex mtx;
        std::condition_variable cv;

        const uint32_t id;
        static uint32_t id_counter;

        std::atomic<status_t> status;
    }; 

    class scheduler
    {
    public:
        typedef std::vector<user_t*> users_t;
        typedef std::vector<process_t*> processes_t;

    private:
        users_t users;
        processes_t processes;

        const size_t quantum;

        std::thread scheduler_thread;
    public:
        scheduler(size_t quantum): quantum(quantum){}

        void create_thread();

        void start();
        void pause();
        void terminate();

        void register_user(const std::string &name)
        {
            this->users.push_back(new user_t(name, this->quantum));
        }

        void register_process(user_t * user, size_t arrival_time, size_t service_time)
        {
            
            size_t index = 0;
            if((index = find_user(user)) < 0)
                throw exceptions::invalid_user();

            this->pause();

            processes.push_back(new process_t(this->users[index], arrival_time, service_time));
            this->users[index]->increment_registered_processes();

            this->start();
        }
        void remove_process(process_t * process)
        {
            
            size_t index = 0;
            if((index = find_process(process)) < 0)
                throw exceptions::invalid_user();

            this->pause();
            
            this->processes[index]->get_user()->decrement_registered_processes();
            delete this->processes[index];
            this->processes.erase(this->processes.begin() + index);

            this->start();
        }

        int32_t find_user(user_t * user)
        {
            for(int32_t i = 0; i < this->users.size(); i++)
                if(*this->users[i] == *user)
                    return i;
            return -1;
        }
        int32_t find_process(process_t * process)
        {
            for(int32_t i = 0; i < this->processes.size(); i++)
                if(*this->processes[i] == *process)
                    return i;
            return -1;
        }

        const users_t& get_users() const {return this->users;}
        const processes_t& get_processes() const {return this->processes;}

        const size_t get_quantum() const {return this->quantum;}
    };
}

#endif