#include <iostream>
#include <cstdlib>
#include <fstream>

#include "parser.hpp"
#include "Writer.hpp"
#include "timer.hpp"

#include "switching.hpp"

int main(int argc, char const *argv[])
{
    // Scheduler use example
    // switching::scheduler scheduler(10, [=](const std::string &msg){std::cout << msg << "\n";});
    // switching::user_t * user = scheduler.register_user("A");
    // switching::user_t * user2 = scheduler.register_user("B");
    // scheduler.register_process(user, 0, 4);
    // scheduler.register_process(user, 1, 4);
    // scheduler.register_process(user2, 3, 4);
    // scheduler.run();
    // scheduler.wait_for_done();

    std::vector<switching::user_t> userList;
    u_int32_t timeQuantum;

    Timer<std::chrono::seconds> timer(1);

    std::string inputFileName = "input.txt";
    if (argc == 2)
    {
        inputFileName = (char *)argv[1];
    }

    Parser parser;

    try
    {
        parser.openFile(inputFileName);
    }
    catch(const std::exception &exception)
    {
        std::cout << exception.what() << std::endl;
    }

    timeQuantum = parser.getTimeQuantum();
    switching::scheduler scheduler(timeQuantum);

    parser.parse(userList, scheduler);

    std::string output_path = "output.txt";
    size_t slash_index = 0;
    if (
        ((slash_index = inputFileName.rfind('/')) != std::string::npos) ||
        ((slash_index = inputFileName.rfind('\\')) != std::string::npos))
        output_path = inputFileName.substr(0, slash_index + 1) + output_path;

    // std::vector<User> userList;
    // u_int32_t timeQuantum;

    // Timer<std::chrono::seconds> timer(1);

    // std::string inputFileName = "input.txt";
    // if (argc == 2)
    // {
    //     inputFileName = (char *)argv[1];
    // }

    // try
    // {
    //     Parser parser(inputFileName);
    //     parser.parse(userList, timeQuantum);
    // }
    // catch (const char *exception)
    // {
    //     std::cout << exception << std::endl;
    //     return EXIT_FAILURE;
    // }

    // std::string output_path = "output.txt";
    // size_t slash_index = 0;
    // if (
    //     ((slash_index = inputFileName.rfind('/')) != std::string::npos) ||
    //     ((slash_index = inputFileName.rfind('\\')) != std::string::npos))
    //     output_path = inputFileName.substr(0, slash_index + 1) + output_path;

    // Writer<std::chrono::seconds> writer(&timer);
    // try
    // {
    //     writer.openFile(output_path);
    // }
    // catch (const char *e)
    // {
    //     std::cout << e << std::endl;
    // }

    scheduler.run();
    scheduler.wait_for_done();

    //timer.startTimer();

    //writter.fileOutput("B", 1, P_START);
    //writter.fileOutput("A", 69, P_FINISH);

    //timer.stopTimer();

    return EXIT_SUCCESS;
}
