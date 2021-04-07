#ifndef _VMM_HPP_
#define _VMM_HPP_

#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

namespace vmm
{
    static const char FILE_TAG = ',';                                   // Used to mark limits in virtual memory file
    typedef unsigned int uint;
    struct page_t                                                       // Holds page info
    {
        int val;
        int id;
        uint last_access_time;

        operator std::string() const {                                  // Mostly for debugging. Used to print the values of page_t
            std::stringstream ss;
            ss << id << ", " << val << ", " << last_access_time;
            return ss.str();
        }
    };

    class vmem_f
    {
    private:
        typedef std::vector<page_t> pages_t;
    public:
        vmem_f() {}                                                     // Default constructor
        vmem_f(const vmem_f& v) { this->pages = v.pages; }              // Copy contructor
        
        void read(const std::string& path);                             // Parses file in path and writes results to this->pages
        void write(const std::string& path);                            // Writes the contents of this->pages into a file

        pages_t& get_pages() { return this->pages; }                    // Get reference to this->pages vector

        void add_page( const page_t& page )                             // Adds page to pages
            { this->pages.push_back(page); }
        page_t& retrieve_page( size_t i )                               // Retrieve page at index
            { return this->pages.at(i); }
        size_t size()                                                   // Get pages size
            { return this->pages.size(); }
    private:
        std::vector<page_t> pages;                                      // Stores parsed/defined pages
    };
}

#endif