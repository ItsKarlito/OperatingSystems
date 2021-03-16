#ifndef PROCESS_T_HPP_
#define PROCESS_T_HPP_

#include "switching.hpp"
#include "thread_controller.hpp"
#include "user_t.hpp"

//#define PROCESS_DEBUG

namespace switching 
{
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

        void set_virgin(bool v);
        bool is_virgin();

    private:
        user_t* user;
        std::atomic<size_t> service_time;
        const size_t arrival_time;

        const uint32_t id;
        static uint32_t id_counter;

        bool virgin;
    };
}

#endif