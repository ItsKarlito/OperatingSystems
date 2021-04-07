#include "vmm.hpp"

namespace vmm
{
    void vmem_f::write(const std::string& path)
    {
        std::ofstream f(path, std::ofstream::out | std::ofstream::binary);
        if(!f.is_open())
            throw std::runtime_error("cannot write to \"" + path + "\"");

        char* buffer = (char*)malloc(sizeof(page_t)*pages.size() + sizeof(char));
        size_t index = 0;
        for(const page_t& page : this->pages)
        {
            buffer[index++] = FILE_TAG;
            memcpy((void *)&buffer[index], (void *)&page, sizeof(page_t));
            index += sizeof(page_t);
        }
        f.write(buffer, index+1);
        free((void*)buffer);
        f.close();
    }

    void vmem_f::read(const std::string& path)
    {
        std::ifstream f(path, std::ifstream::in | std::ifstream::binary | std::ifstream::ate);
        if(!f.is_open())
            throw std::runtime_error("cannot read \"" + path + "\"");

        this->pages.clear();

        size_t file_size = f.tellg();
        f.clear();
        f.seekg(0, std::ifstream::beg);

        if(file_size < 1)
        {
            f.close();
            return;
        }

        std::string buffer;
        buffer.reserve(file_size);        
        buffer.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        f.close();

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
}