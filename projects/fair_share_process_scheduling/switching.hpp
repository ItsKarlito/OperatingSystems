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

    class utilities
    {
        static size_t get_time() {return time(NULL);}
    };

    class exceptions
    {
    public:
        struct null_pointer_error : public std::exception
        {const char * what () const throw () {return "null referenced passed to object";}};
        struct index_out_of_bounds : public std::runtime_error
        {const char * what () const throw () {return "index points outside of allocated memory space";}};

    };
    class user_t
    {
    public:
        user_t(const std::string &name): name(name), id(id_counter++){}

        uint32_t get_id() const {return id;} 
    private:
        const uint32_t id;
        static uint32_t id_counter;
        std::string name;
    };
    
    class process_t 
    {
    public:
        enum status_t
        {
            RUNNING,
            IDLE,
            TERMINATED
        };

    public:
        process_t(const user_t* user);

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
        void terminate() {this->set_status(status_t::TERMINATED);}

        size_t get_arrival_time() const {return this->arrival_time;}
        size_t get_service_time() const {return this->service_time;}

        void set_service_time(size_t service_time) {this->service_time = service_time;}

    private:
        void create_thread();

    private:
        const user_t* user;
        std::atomic<size_t> service_time;
        size_t arrival_time;

        std::thread thread;
        std::mutex mtx;
        std::condition_variable cv;

        const uint32_t id;
        static uint32_t id_counter;

        status_t status;
    }; 

    class scheduler
    {
    public:
        typedef std::vector<user_t*> users_t;
        typedef std::vector<process_t*> processes_t;

    private:
        users_t users;
        processes_t processes;
    public:
        scheduler(){}

        void start();
        void stop();

        void register_user(user_t * user);
        void register_proces(user_t * user, process_t * process);

        int32_t find_user(user_t * user);
        int32_t find_process(process_t * process);

        const users_t& get_users() const {return this->users;}
        const processes_t& get_processes() const {return this->processes;}
    };
}

#endif