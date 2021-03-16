#ifndef SCHEDULER_HPP_
#define SCHEDULER_HPP_

#include "switching.hpp"
#include "thread_controller.hpp"
#include "user_t.hpp"
#include "process_t.hpp"

namespace switching 
{
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

        user_t * register_user(const std::string &name);

        process_t * register_process(user_t * user, size_t arrival_time, size_t service_time);
        void remove_process(process_t * process);

        int32_t find_user(user_t * user);
        int32_t find_process(process_t * process);

        const users_t& get_users() const;
        const processes_t& get_processes() const;

        const size_t get_quantum() const;
    };
}

#endif