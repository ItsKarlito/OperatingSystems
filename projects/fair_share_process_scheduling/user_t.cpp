#include "user_t.hpp"

namespace switching
{
    uint32_t user_t::id_counter = 0;
    
    user_t::user_t(const std::string &name, size_t quantum): 
    name(name), id(id_counter++), burst(0), registered_processes(0), quantum(quantum) {}

    uint32_t user_t::get_id() const { return id; }

    size_t user_t::get_burst() const { return this->burst; }
    void user_t::update_burst() 
    {
        this->burst = this->quantum/this->registered_processes;
        this->burst = this->burst < 0 ? 1: this->burst;
    }

    void user_t::increment_registered_processes() 
    {
        this->set_registered_processes(this->registered_processes++);
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
}