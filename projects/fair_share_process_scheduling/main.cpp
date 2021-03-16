#include <iostream>
#include <cstdlib>
#include <fstream>

#include "scheduler.hpp"

int main(int argc, char const *argv[])
{
    switching::scheduler scheduler(10, [=](const std::string &msg){std::cout << msg << "\n";});
    switching::user_t * user = scheduler.register_user("A");
    switching::user_t * user2 = scheduler.register_user("B");
    scheduler.register_process(user, 0, 4);
    scheduler.register_process(user, 1, 4);
    scheduler.register_process(user2, 3, 4);
    scheduler.run();
    scheduler.wait_for_done();

    return EXIT_SUCCESS;
}
