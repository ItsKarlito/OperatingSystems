#include "switching.hpp"
#include <exception>

namespace switching
{
    uint32_t user_t::id_counter = 0;
    uint32_t process_t::id_counter = 0;

    process_t::process_t(const user_t* user, size_t arrival_time, size_t service_time): 
        user(user), id(id_counter++), status(status_t::IDLE), arrival_time(arrival_time), service_time(service_time)
    {
        if(this->user == nullptr)
            throw exceptions::null_pointer_error();
        
        this->create_thread();
    }

    process_t::~process_t()
    {
        if(!this->thread.joinable())
            return;
        this->terminate();
        this->thread.join();
    }

    void process_t::create_thread()
    {
        if(this->thread.joinable())
            return;
        
        this->thread = std::thread([&](){
            bool alive = true;
            while(alive)
            {
                std::unique_lock<std::mutex> lck(this->mtx);
                while(this->status == status_t::IDLE) cv.wait(lck);

                switch (this->status)
                {
                case status_t::IDLE:
                    break;
                case status_t::TERMINATED:
                    alive = false;
                    break;                
                case status_t::RUNNING:
                    if(this->service_time > 0)
                    {
                        std::this_thread::sleep_for(time_unit_t(1));
                        this->service_time--;
                    }
                    else 
                        alive = false;
                    break;
                }
            }
            this->status = status_t::TERMINATED;
        });
    }
}
