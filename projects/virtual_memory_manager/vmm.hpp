#ifndef _VMM_HPP_
#define _VMM_HPP_

#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <memory>
#include <functional>
#include <queue>

#include "thread_controller.hpp"

namespace vmm
{
    static const char FILE_TAG = ',';                                   // Used to mark limits in virtual memory file
    typedef unsigned int uint;
    struct page_t                                                       // Holds page info
    {
        int val = -1;
        std::string id = "";
        uint last_access_time = 0;

        bool available = false;

        operator std::string() const {                                  // Mostly for debugging. Used to print the values of page_t
            std::stringstream ss;
            ss << id << ", " << val << ", " << last_access_time;
            return ss.str();
        }
    };

    class vmm_f
    {
    public:
        typedef std::vector<page_t> pages_t;
        std::string path;

    public:
        vmm_f() {}                                                     // Default constructor
        vmm_f(const std::string &path): path(path) {}                                 
        vmm_f(const vmm_f& v) { this->pages = v.pages; }              // Copy contructor
        
        void read(const std::string& path);                             // Parses file in path and writes results to this->pages
        void write(const std::string& path);                            // Writes the contents of this->pages into a file
        void read();                                                    // Parses file in path and writes results to this->pages
        void write();                                                   // Writes the contents of this->pages into a file
        void remove( size_t i );

        pages_t& get_pages() { return this->pages; }                    // Get reference to this->pages vector

        void add_page( const page_t& page );                            // Adds page to pages
        page_t& retrieve_page( size_t i );                              // Retrieve page at index
        size_t size();                                                  // Get pages size

    private:
        std::vector<page_t> pages;                                      // Stores parsed/defined pages
    };

    class vmm_manager
    {
    public:
        typedef std::function<void(std::string msg)> logger_callback_t;
        typedef std::function<uint()> timer_callback_t;

    private:
        vmm_f file;
        std::string file_path;
        std::vector<page_t> pages;
        size_t page_count = 0;

        logger_callback_t logger_callback;
        timer_callback_t timer_callback;

    private:
        void log(std::string msg);
        size_t get_time();
    
    public:
        vmm_manager(size_t page_count, std::string file_path);
        
        void store(std::string id, uint value);
        bool release(std::string id);
        long int lookup(std::string id);

        void set_logger_callback(logger_callback_t c);
        void set_timer_callback(timer_callback_t c);
    };

    class vmm: public thread_controller
    {
    private:
        vmm_manager manager;
        enum operation_type
        {
            store_op,
            release_op,
            lookup_op
        };
        struct operation
        {
            operation_type type;

            std::string id = "";
            uint val = 0;

            bool served = true;
        };

        uint call_id;
        std::mutex operation_buffer_mutex;
        operation operation_buffer;
        bool op_ready;
        std::condition_variable op_cv;

        std::mutex resp_mutex;
        long int resp = -1;
        bool resp_ready = false;
        std::condition_variable resp_cv;
    
    public:
        vmm(size_t page_count, std::string file_path);
        ~vmm();

        void store(std::string id, uint value);
        bool release(std::string id);
        long int lookup(std::string id);

        void set_logger_callback(vmm_manager::logger_callback_t c);
        void set_timer_callback(vmm_manager::timer_callback_t c);

    protected:
        void cycle() override;
    
    private:
        void set_response(long int resp);
        long int get_response();

        void set_operation(const operation& op);
        operation get_operation();
    };
}

#endif