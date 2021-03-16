#include <iostream>
#include <cstdlib>
#include <fstream>

#include "scheduler.hpp"

int main(int argc, char const *argv[])
{
    switching::user_t user("testing", 10);
    switching::process_t process(&user, 0, 10);
    process.run();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "RUNNING\n";
    process.pause();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    process.run();
    std::this_thread::sleep_for(std::chrono::seconds(12));
    process.terminate();

    return EXIT_SUCCESS;
}
