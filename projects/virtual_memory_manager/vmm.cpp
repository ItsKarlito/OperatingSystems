#include "vmm.hpp"

namespace vmm
{
    vmm::vmm(size_t page_count, std::string file_path, CommandBuffer* command_buffer): 
        manager(page_count, file_path), command_buffer(command_buffer) 
    {
        this->create_thread();

        std::ofstream f(file_path, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
        f.close();
    }
    vmm::~vmm()
    {
        this->terminate();
    }

    void vmm::cycle()
    {
        // If the command buffer is empty and you're not told to terminate
        // just don't do anything. If you are told to terminate, exit the
        // function.
        while(this->command_buffer->getSize() < 1)
        {
            if(this->get_status() == thread_controller::status_t::TERMINATED)
                return;
        }

        Parser::Command cmd = command_buffer->popCmd();
        
        switch (cmd.name)
        {
        case Parser::command_type::LOOKUP:
            this->manager.lookup(std::to_string(cmd.id));
            break;
        case Parser::command_type::RELEASE:
            this->manager.release(std::to_string(cmd.id));
            break;
        case Parser::command_type::STORE:
            this->manager.store(std::to_string(cmd.id), cmd.value);
            break;
        case Parser::command_type::ERROR:
            break;
        }
    }

    void vmm::set_logger_callback(vmm_manager::logger_callback_t c)
    {
        this->manager.set_logger_callback(c);
    }
    void vmm::set_timer_callback(vmm_manager::timer_callback_t c)
    {
        this->manager.set_timer_callback(c);
    }
}