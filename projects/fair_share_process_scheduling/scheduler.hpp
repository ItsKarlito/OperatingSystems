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
        typedef const std::function<void(const std::string&)> logger_t;

    private:
        users_t users;
        processes_t processes;

        const size_t quantum;

        size_t current_process;
        logger_t logger;

        std::condition_variable done_cv;
        std::mutex done_mtx;
        bool done;
    public:
        scheduler(size_t quantum, logger_t logger);
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

        void update_quantums();

        virtual void cycle() override;

        void log(const std::string &msg) const;

        void wait_for_done();
    };
}

#endif