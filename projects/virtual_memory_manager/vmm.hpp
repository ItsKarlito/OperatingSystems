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
    static const char FILE_TAG = ',';
    typedef unsigned int uint;
    struct page_t
    {
        int val;
        int id;
        uint last_access_time;

        operator std::string() const {
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
        vmem_f() {}
        
        void read(const std::string& path);
        void write(const std::string& path);

        pages_t& get_pages() { return this->pages; }
    private:
        std::vector<page_t> pages;
    };
}

#endif