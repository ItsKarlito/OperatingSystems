#ifndef SWITCHING_HPP_
#define SWITCHING_HPP_

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdint>
#include <vector>
#include <atomic>
#include <exception>
#include <chrono>
#include <functional>

#include "Writer.hpp"

namespace switching 
{
    //*******************************************************************************
    //************************************GLOBALS************************************
    //*******************************************************************************
    typedef std::chrono::seconds time_unit_t;                                                           // Define default time unit (currently set to seconds)

    enum status_t                                                                                       // Used to define process status (shared by thread_controller)
    {
        RUNNING,
        IDLE,
        TERMINATED
    };

    class exceptions                                                                                    // Custom exceptions for process, user and data handling
    {
    public:
        struct null_pointer_error : public std::exception
        {const char * what () const throw () {return "null referenced passed to object";}};
        struct invalid_user : public std::exception
        {const char * what () const throw () {return "specified user has not been registered";}};
        struct invalid_process : public std::exception
        {const char * what () const throw () {return "specified process has not been registered";}};

    };

    //*******************************************************************************
    //*********************************USER HANDLING*********************************
    //*******************************************************************************
    class user_t
    {
    public:
        user_t(const std::string &name, size_t quantum);

        uint32_t get_id() const;

        bool operator==(const user_t& user) const {return this->id == user.get_id();}                   // Used for user searching (see scheduler::find_user)

        size_t get_burst() const;
        void update_burst();                                                                            // Updates the burst based on the current quantum and the amount of users

        void increment_registered_processes();                                                          // Changes the amount of processes registered to the user. I will also automatically updates
        void decrement_registered_processes();                                                          // the burst time as they are used
        void set_registered_processes(size_t registered_processes);
        size_t get_registered_processes() const;

        const std::string& get_name() const;

        void update_quantum(size_t quantum);                                                            // Updates the quantum (and burst)

    private:
        size_t burst;
        size_t quantum;
        size_t registered_processes;
        const uint32_t id;
        static uint32_t id_counter;
        std::string name;
    };

    //*******************************************************************************
    //********************************THREAD HANDLING********************************
    //*******************************************************************************
    class thread_controller
    {
    protected:
        virtual void cycle() = 0;                                                                       // Pure virtual function that serves as the entry point for thread_controller's
                                                                                                        // children
                                                                                                        
        void create_thread();                                                                           // Creates the thread to be controlled (must be called before run/pause/terminate)
        std::thread& get_thread();

    public:
        void set_status(status_t status);                                                               // Changes the control flow for the thread
        status_t get_status() const;
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

    //********************************************************************************
    //********************************PROCESS HANDLING********************************
    //********************************************************************************
    class process_t: public thread_controller
    {
    public:
        process_t(user_t* user, size_t arrival_time, size_t service_time);
        ~process_t();

        size_t get_arrival_time() const;
        size_t get_service_time() const;
        void set_service_time(size_t service_time);

        uint32_t get_id() const;

        bool operator==(const process_t& process) const {return this->id == process.get_id();}          // Used for searching (see scheduler::find_process)

        user_t * get_user();

        virtual void cycle() override;

        void set_virgin(bool v);                                                                        // Used to know if process has been excecuted before
        bool is_virgin();

    private:
        user_t* user;
        std::atomic<size_t> service_time;
        const size_t arrival_time;

        const uint32_t id;
        static uint32_t id_counter;

        bool virgin;
    };

    //*******************************************************************************
    //*******************************SCHEDULE HANDLING*******************************
    //*******************************************************************************
    class scheduler: public thread_controller
    {
    public:
        typedef std::vector<user_t*> users_t;
        typedef std::vector<process_t*> processes_t;

    private:
        users_t users;
        processes_t processes;

        const size_t quantum;

        size_t current_process;
        Writer::writerFunctor_t logger;

        std::condition_variable done_cv;
        std::mutex done_mtx;
        bool done;

    public:
        scheduler(size_t quantum, Writer::writerFunctor_t logger);
        scheduler(size_t quantum);
        ~scheduler();

        user_t * register_user(const std::string &name);                                                    // Adds an user to the register queue

        process_t * register_process(user_t * user, size_t arrival_time, size_t service_time);              // Registers a process on the process queue and update the users
        void remove_process(process_t * process);                                                           // quantums and bursts times

        int32_t find_user(user_t * user);
        int32_t find_process(process_t * process);

        const users_t& get_users() const;
        const processes_t& get_processes() const;

        const size_t get_quantum() const;
        void update_quantums();                                                                             // Updates the quantums for all the users

        virtual void cycle() override;

        void log(std::string userName, int pID, Writer::output_action action);                              // Uses logger to log progress unto a functor

        void wait_for_done();                                                                               // Halts current thread (from were it's called) until scheduler it's done
    };
}

#endif