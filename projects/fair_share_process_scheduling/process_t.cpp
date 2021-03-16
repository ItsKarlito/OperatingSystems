#include "process_t.hpp"

namespace switching
{
    uint32_t process_t::id_counter = 0;
    
    process_t::process_t(user_t* user, size_t arrival_time, size_t service_time):
        user(user), id(id_counter++), arrival_time(arrival_time), service_time(service_time)
    {
        if(this->user == nullptr)
            throw exceptions::null_pointer_error();
        
        this->create_thread();
    }
    process_t::~process_t()
    {
        this->terminate();
    }

    size_t process_t::get_arrival_time() const { return this->arrival_time; }
    size_t process_t::get_service_time() const { return this->service_time; }
    void process_t::set_service_time(size_t service_time) {this->service_time = service_time; }

    uint32_t process_t::get_id() const { return this->id; }

    user_t * process_t::get_user() { return this->user; }

    void process_t::cycle()
    {
        if(this->service_time > 0)
        {
#ifdef PROCESS_DEBUG
            std::cout << "service time: " << this->service_time << "\n";
#endif
            this->service_time--;
            std::this_thread::sleep_for(time_unit_t(1));
        }
        else 
            this->pause();
    }
}