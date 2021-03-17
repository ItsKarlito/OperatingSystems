#include <iostream>
#include <cstdlib>
#include <fstream>

#include "parser.hpp"
#include "Writer.hpp"
#include "timer.hpp"

#include "switching.hpp"

class timer : public switching::thread_controller
{
public:
    struct timer_entry
    {
        timer_entry(const Parser::Process& process, switching::user_t* user):
        process(process), user(user) {}
        Parser::Process process;
        switching::user_t* user;
    };
    timer(switching::scheduler* scheduler, const std::vector<timer_entry>& data):
    scheduler(scheduler), data(data) 
    {
        this->create_thread();
    }

private:
    switching::scheduler* scheduler;
    const std::vector<timer_entry>& data;
    size_t counter = 1;
    virtual void cycle() override
    {
        for(const timer_entry& entry : data)
        {
            if(entry.process.arrivalTime == counter)
                scheduler->register_process(entry.user, entry.process.arrivalTime, entry.process.serviceTime);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        counter++;
    }
};

int main(int argc, char const *argv[])
{
    std::string inputFileName = "input.txt";
    if (argc == 2)
    {
        inputFileName = (char *)argv[1];
    }
    Parser parser(inputFileName);


    std::string output_path = "output.txt";
    size_t slash_index = 0;
    if (
        ((slash_index = inputFileName.rfind('/')) != std::string::npos) ||
        ((slash_index = inputFileName.rfind('\\')) != std::string::npos))
        output_path = inputFileName.substr(0, slash_index + 1) + output_path;
    Writer writer;

    try
    {
        parser.parse();
        writer.openFile(output_path);
    }
    catch(const std::exception &exception)
    {
        std::cout << "Error: " << exception.what() << std::endl;
        return 1;
    }

    Parser::Data parser_data = parser.getData();
    switching::scheduler scheduler(parser_data.timeQuantum, [&](std::string userName, int pID, Writer::output_action action){
        writer.fileOutput(userName, pID, action);
    });

    std::vector<timer::timer_entry> entries = {};
    for(const Parser::User& user : parser_data.users)
    {
        switching::user_t * ptr = scheduler.register_user(user.name);
        for(const Parser::Process& process : user.processes)
            entries.push_back(timer::timer_entry(process, ptr));
    }
    timer scheduler_timer(&scheduler, entries);
    writer.set_offset();
    scheduler_timer.run();
    scheduler.run();
    scheduler.wait_for_done();
    scheduler_timer.terminate();

    return 0;
}
