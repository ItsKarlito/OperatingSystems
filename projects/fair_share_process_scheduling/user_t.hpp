#ifndef USER_T_HPP_
#define USER_T_HPP_

#include "switching.hpp"

namespace switching 
{
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
}

#endif