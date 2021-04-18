#include "vmm.hpp"

namespace vmm
{
    void vmm_f::add_page( const page_t& page )
    {
        this->pages.push_back(page);
    }
    page_t& vmm_f::retrieve_page( size_t i )
    {
        return this->pages.at(i);
    }
    size_t vmm_f::size()
    {
        return this->pages.size();
    }
    
    void vmm_f::write(const std::string& path)
    {
        // Open file, or throw an exception if you can't open it
        std::ofstream f(path, std::ofstream::out | std::ofstream::binary);
        if(!f.is_open())
            throw std::runtime_error("cannot write to \"" + path + "\"");

        // Allocate some memory space for the file data
        char* buffer = (char*)malloc(sizeof(page_t)*pages.size());

        // Write this->pages info into the array
        size_t index = 0;
        for(const page_t& page : this->pages)
        {
            buffer[index++] = FILE_TAG;
            memcpy((void *)&buffer[index], (void *)&page, sizeof(page_t));
            index += sizeof(page_t);
        }

        // Write array into file
        f.write(buffer, index+1);

        // Free array and close file
        free((void*)buffer);
        f.close();
    }

    void vmm_f::read(const std::string& path)
    {
        // Open file for reading or throw an exception if you can't. Also, reset the file pointer to
        // the end to get the size
        std::ifstream f(path, std::ifstream::in | std::ifstream::binary | std::ifstream::ate);
        if(!f.is_open())
            throw std::runtime_error("cannot read \"" + path + "\"");

        // Delete previous pages entries
        this->pages.clear();

        // Get the file size with the file pointer
        size_t file_size = f.tellg();

        // Reset the file pointer to the beggining of the file
        f.clear();
        f.seekg(0, std::ifstream::beg);
        
        // Check if the file has a size larger than one
        if(file_size < 1)
        {
            f.close();
            return;
        }

        // Read the file contents into memory (buffer), and close the file since
        // we will be no longer using it
        std::string buffer;
        buffer.reserve(file_size);        
        buffer.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        f.close();

        // Go through the file. If you find a ',' and you did so after
        // sizeof(page_t)+1 amount for bytes has passed, copy the bytes
        // in between the current and last ',' into a page_t object. Then
        // append that object into this->pages.
        std::string s_b = "";
        page_t t = {};
        size_t safe = 0;
        const size_t b_s = buffer.size();
        for(size_t i = 0; i < b_s; i++)
        {
            char c = buffer[i];
            if((c == FILE_TAG && i >= safe) || i == b_s-1)
            {
                safe += sizeof(page_t)+1;
                if(i < 1)
                    continue;

                memcpy((void *) &t, (void*)s_b.c_str(), sizeof(page_t));
                this->pages.push_back(t);
                s_b = "";
            }
            else
                s_b += c;
        }
    }
    
    void vmm_f::read()
    { this->read(this->path); }

    void vmm_f::write()
    { this->write(this->path); }

    void vmm_f::remove(size_t i)
    {
        if(i < this->pages.size())
            this->pages.erase(this->pages.begin() + i);
    }
}