#ifndef THREAD_CONTROLLER_
#define THREAD_CONTROLLER_

#include "switching.hpp"

namespace switching 
{    
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
}

#endif