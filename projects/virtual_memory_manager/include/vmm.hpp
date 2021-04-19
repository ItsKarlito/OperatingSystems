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
#include "commandbuffer.hpp"
#include "timer.hpp"
#include "writer.hpp"

namespace vmm
{
    static const char FILE_TAG = ',';
    typedef unsigned int uint;
    struct page_t
    {
        int val = -1;
        std::string id = "";
        uint last_access_time = 0;

        bool available = true;
        
        // Used for debug
#ifndef VMM_DEBUG
        operator std::string() const {
            std::stringstream ss;
            ss << id << ", " << val << ", " << last_access_time;
            return ss.str();
        }
#endif
    };

    class vmm_file
    {
    public:
        typedef std::vector<page_t> pages_t;
        std::string path;

    public:
        // Default constructor
        vmm_file() {}

        // Uses path to set the virtual memory file for the 
        // write and read functions
        vmm_file(const std::string &path): path(path) {}

        // Copy constructor
        vmm_file(const vmm_file& v) { this->pages = v.pages; }
        
        // Reads the file contents and gets the pages out of it and
        // stores them into this->pages if any are found
        void read(const std::string& path);
        // Write the pages from this->pages into the file in path
        void write(const std::string& path);

        // Same as read(this->path)
        void read();
        // Same as write(this->path)
        void write();
        void remove( size_t i );

        // Returns a reference to this->pages
        pages_t& get_pages() { return this->pages; }

        // Adds page to this->pages
        void add_page( const page_t& page );
        // Retrieve i element from this->pages
        page_t& retrieve_page( size_t i );

    private:
        std::vector<page_t> pages;
    };

    class vmm_manager
    {
    public:
        typedef std::function<void(std::string msg)> logger_callback_t;
        typedef std::function<uint()> timer_callback_t;

    private:
        vmm_file file;
        std::string file_path;
        page_t* pages;
        size_t page_count = 0;

        logger_callback_t logger_callback;
        timer_callback_t timer_callback;

    private:
        // Log msg using logger_callback. Do nothing
        // if it's not defined
        void log(std::string msg);

        // Get time using timer_callback. Return 0 if
        // timer_callback if it's not defined
        size_t get_time();
    
    public:

        // Constructor. page_count defines the number of pages
        // in virtual memory and file_path defines the path
        // for the physical memory file
        vmm_manager(size_t page_count, std::string file_path);

        // Destructor. Frees this->pages.
        ~vmm_manager();
        
        // Stores value and id in the next available spot on
        // memory. If no spot is available, it will save it
        // into physical memory
        void store(std::string id, uint value);

        // Frees virtual memory page with id
        bool release(std::string id);
        
        // Retrieves value from memory.
        long int lookup(std::string id);

        // Set callback (void(std::string)) for logger
        void set_logger_callback(logger_callback_t c);

        // Set callback (size_t()) for timer. timer is used
        // to get the real time
        void set_timer_callback(timer_callback_t c);
    };

    class vmm_thread: public thread_controller
    {
    private:
        vmm_manager manager;
        Writer* writer;
        Timer<std::chrono::milliseconds>* timer;
        CommandBuffer* command_buffer;
    
    public:
        // Constructor. page_count is used for the amount of pages in
        // virtual memory. file_path is the path for the physical memory
        // file. command_buffer is a pointer for a command_buffer instance.
        //
        // It also creates the thread for the manager
        vmm_thread(
            size_t page_count, 
            std::string file_path, 
            CommandBuffer* command_buffer,
            Timer<std::chrono::milliseconds>* timer,
            Writer* writer
        );

        // Destructor. Terminates thread
        ~vmm_thread();

    protected:

        // Will go through the command_buffer's buffer
        // and excecute its instructions. It's also thread
        // safe
        void cycle() override;
    };
}

#endif