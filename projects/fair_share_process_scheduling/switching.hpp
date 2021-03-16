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
        user_t(const std::string &name, size_t quantum);

        uint32_t get_id() const;

        bool operator==(const user_t& user) const {return this->id == user.get_id();} 

        size_t get_burst() const;
        void update_burst();

        void increment_registered_processes();
        void decrement_registered_processes();

        size_t get_registered_processes() const;
        void set_registered_processes(size_t registered_processes);

        const std::string& get_name() const;

    private:
        size_t burst;
        const size_t quantum;
        size_t registered_processes;
        const uint32_t id;
        static uint32_t id_counter;
        std::string name;
    };
    
    class thread_controller
    {
    protected:
        virtual void cycle() = 0;

        void create_thread();
        std::thread& get_thread();

        status_t get_status() const;
        void set_status(status_t status);

        void run();
        void pause();
        void terminate();

    private:
        std::thread thread;
        std::mutex mtx;
        std::atomic<status_t> status;
        std::atomic<bool> alive;
        std::condition_variable cv;
    };

    class process_t: public thread_controller
    {
    public:
        process_t(user_t* user, size_t arrival_time, size_t service_time);
        ~process_t();

        size_t get_arrival_time() const;
        size_t get_service_time() const;
        void set_service_time(size_t service_time);

        uint32_t get_id() const;

        bool operator==(const process_t& process) const {return this->id == process.get_id();}

        user_t * get_user();

        virtual void cycle() override;

    private:
        user_t* user;
        std::atomic<size_t> service_time;
        const size_t arrival_time;

        const uint32_t id;
        static uint32_t id_counter;
    }; 

    class scheduler: public thread_controller
    {
    public:
        typedef std::vector<user_t*> users_t;
        typedef std::vector<process_t*> processes_t;

    private:
        users_t users;
        processes_t processes;

        const size_t quantum;
    public:
        scheduler(size_t quantum);
        ~scheduler();

        void register_user(const std::string &name);

        void register_process(user_t * user, size_t arrival_time, size_t service_time);
        void remove_process(process_t * process);

        int32_t find_user(user_t * user);
        int32_t find_process(process_t * process);

        const users_t& get_users() const;
        const processes_t& get_processes() const;

        const size_t get_quantum() const;
    };
}

#endif