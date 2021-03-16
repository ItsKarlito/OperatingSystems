#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

template <typename time_unit = std::chrono::seconds>
class Timer
{
private:
    u_int64_t elapsedTime;
    u_int64_t delay;
    std::atomic<bool> isRun;
    std::thread timerThread;

public:
    Timer(u_int64_t Delay)
    {
        elapsedTime = 1;
        delay = Delay;
        isRun = false;
    }

    void startTimer()
    {
        if (this->timerThread.joinable())
            return;

        isRun = true;
        timerThread = std::thread([&]() {
            while (isRun)
            {
                std::cout << elapsedTime << '\n';
                std::this_thread::sleep_for(time_unit(delay));
                elapsedTime++;
            }
        });
    }

    void stopTimer()
    {
        isRun = false;
    }

    u_int64_t getElapsedTime() { return elapsedTime; }

    ~Timer()
    {
        if (this->timerThread.joinable())
            this->timerThread.join();
    }
};

#endif
