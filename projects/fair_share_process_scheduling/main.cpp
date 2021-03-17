#include <iostream>
#include <cstdlib>
#include <fstream>

#include "parser.hpp"
#include "Writer.hpp"
#include "timer.hpp"

#include "switching.hpp"

int main(int argc, char const *argv[])
{
    //Process CLI arguments, open file, check if successful
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

    //Parse file and setup writer//Parse file and setup writer
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

    //Transfer parsed data to the scheduler
    Parser::Data parser_data = parser.getData();
    switching::scheduler scheduler(parser_data.timeQuantum, [&](std::string userName, int pID, Writer::output_action action){
        writer.fileOutput(userName, pID, action);
    });
    struct timer_entry
    {
        timer_entry(const Parser::Process& process, switching::user_t* user):
        process(process), user(user) {}
        Parser::Process process;
        switching::user_t* user;
    };
    std::vector<timer_entry> entries = {};
    for(const Parser::User& user : parser_data.users)
    {
        switching::user_t * ptr = scheduler.register_user(user.name);
        for(const Parser::Process& process : user.processes)
            entries.push_back(timer_entry(process, ptr));
    }
    writer.set_offset(); //sets the unix time reference
    scheduler.run(); //run the scheduler
    size_t counter = 1;
    size_t registered = 0;
    while(entries.size() > registered)
    {
        for(const timer_entry& entry : entries)
        {
            if(entry.process.arrivalTime == counter)
            {
                scheduler.register_process(entry.user, entry.process.arrivalTime, entry.process.serviceTime);
                registered++;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        counter++;
    }
    scheduler.wait_for_done(); //wait for scheduler to finish

    return 0;
}
