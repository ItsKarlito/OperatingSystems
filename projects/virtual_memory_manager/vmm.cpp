#include "vmm.hpp"

namespace vmm
{
    vmm::vmm(size_t page_count, std::string file_path): manager(page_count, file_path) {}

    void vmm::cycle()
    {
        operation op = this->get_operation();
        switch (op.type)
        {
        case operation_type::store_op:
            this->manager.store(op.id, op.type);
            this->set_response(0);
            break;
        case operation_type::release_op:
            this->set_response(this->manager.release(op.id));
            break;        
        case operation_type::lookup_op:
            this->set_response(this->manager.lookup(op.id));
            break;
        }
    }

    void vmm::store(std::string id, uint value)
    {
        operation op = {};
        op.id = id;
        op.val = value;
        op.type = operation_type::store_op;
        this->set_operation(op);
        this->get_response();
    }
    bool vmm::release(std::string id)
    {
        operation op = {};
        op.id = id;
        op.type = operation_type::store_op;
        this->set_operation(op);
        return static_cast<bool>(this->get_response());
    }
    long int vmm::lookup(std::string id)
    {
        operation op = {};
        op.id = id;
        op.type = operation_type::store_op;
        this->set_operation(op);
        return this->get_response();
    }

    void vmm::set_logger_callback(vmm_manager::logger_callback_t c)
    {
        this->manager.set_logger_callback(c);
    }
    void vmm::set_timer_callback(vmm_manager::timer_callback_t c)
    {
        this->manager.set_timer_callback(c);
    }

    void vmm::set_response(long int resp)
    {
        std::unique_lock<std::mutex> lck(this->resp_mutex);
        this->resp = resp;
        this->resp_ready = true;
        lck.unlock();
        resp_cv.notify_all();
    }

    long int vmm::get_response()
    {
        std::unique_lock<std::mutex> lck(this->resp_mutex);
        this->resp_ready = false;
        resp_cv.wait(lck, [this]{return this->resp_ready;});
        return resp;
    }

    void vmm::set_operation(const operation& op)
    {
        std::unique_lock<std::mutex> lck(this->operation_buffer_mutex);
        this->operation_buffer = op;
        this->op_ready = true;
        lck.unlock();
        op_cv.notify_all();
    }

    vmm::operation vmm::get_operation()
    {
        std::unique_lock<std::mutex> lck(this->operation_buffer_mutex);
        this->op_ready = false;
        op_cv.wait(lck, [this]{return this->resp_ready;});
        return operation_buffer;
    }
}