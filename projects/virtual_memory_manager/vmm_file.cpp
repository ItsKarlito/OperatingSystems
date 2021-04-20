#include "vmm.hpp"

namespace vmm
{
    void vmm_file::add_page(const page_t &page)
    {
        this->pages.push_back(page);
    }

    void vmm_file::write(const std::string &path)
    {
        // Open file, or throw an exception if you can't open it
        std::ofstream f(path, std::ofstream::out | std::ofstream::binary);
        if (!f.is_open())
            throw std::runtime_error("cannot write to \"" + path + "\"");

        // Write the contents of the page into a string and write that string
        // into a file
        std::string buffer;
        for (int i = 0; i < this->pages.size(); i++)
        {
            buffer = ",";
            buffer += this->pages[i].id;
            buffer += "|";
            buffer += std::to_string(this->pages[i].val);
            buffer += "|";
            buffer += std::to_string(this->pages[i].last_access_time);
            buffer += "|";
            buffer += std::to_string(this->pages[i].available);
        }
        buffer += ",";
        f.write(buffer.c_str(), buffer.size());

        // Close file
        f.close();
    }

    void vmm_file::read(const std::string &path)
    {
        // Open file for reading or throw an exception if you can't. Also, reset the file pointer to
        // the end to get the size
        std::ifstream f(path, std::ifstream::in | std::ifstream::binary | std::ifstream::ate);
        if (!f.is_open())
            throw std::runtime_error("cannot read \"" + path + "\"");

        // Delete previous pages entries
        this->pages.clear();

        // Get the file size with the file pointer
        size_t file_size = f.tellg();

        // Reset the file pointer to the beggining of the file
        f.clear();
        f.seekg(0, std::ifstream::beg);

        // Check if the file has a size larger than one
        if (file_size < 1)
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

        // Find the commas on the file and use them to know the beggining and ends
        // of each page
        page_t t = {};
        for (size_t i = 0; i < file_size; i++)
        {
            if (buffer[i] == ',')
            {
                size_t s = ++i;
                for (; buffer[s] != '|' && s < file_size; s++);
                t.id = std::string(&buffer[i], s++ - i);
                i = s;
                for (; buffer[s] != '|' && s < file_size; s++);
                t.val = std::stol(std::string(&buffer[i], ++s - i));
                i = s;
                for (; buffer[s] != '|' && s < file_size; s++);
                t.last_access_time = std::stol(std::string(&buffer[i], ++s - i));
                i = s;
                for (; buffer[s] != ',' && s < file_size; s++);
                t.available = static_cast<bool>(std::stol(std::string(&buffer[i], ++s - i)));
                this->pages.push_back(t);
                i = s;
            }
        }
    }

    void vmm_file::read()
    {
        this->read(this->path);
    }

    void vmm_file::write()
    {
        this->write(this->path);
    }

    void vmm_file::remove(size_t i)
    {
        if (i < this->pages.size())
            this->pages.erase(this->pages.begin() + i);
    }
}