#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

template <typename time_unit = std::chrono::milliseconds> //use template to indicate the time unit
class Timer
{
private:
    u_int64_t elapsedTime;      //time elapsed from beginning of simulation
    u_int64_t delay;            //time frame for which the timer will sleep
    std::atomic<bool> isRun;    //boolean value indicating the thread that it needs to be running or not
    std::thread timerThread;    //timer thread
    std::mutex timerMutex;      //elapsed time mutex

public:
    Timer(u_int64_t Delay)
    {
        elapsedTime = 1;
        delay = Delay;
        isRun = false;
    }

    //timer thread 
    void startTimer()
    {
        //check if timer thread already started
        if (this->timerThread.joinable())
            return;

        isRun = true;
        timerThread = std::thread([&]() {
            while (isRun)
            {
                //std::cout << elapsedTime << '\n';
                std::this_thread::sleep_for(time_unit(delay)); //sleep for the indicated time frame

                std::unique_lock<std::mutex> lck(timerMutex);
                elapsedTime++;  //increase execution time
                lck.unlock();
            }
        });
    }

    void stopTimer()
    {
        isRun = false;  //tell timer thread to stop executing
    }

    u_int64_t getElapsedTime()
    {
        std::unique_lock<std::mutex> lck(timerMutex);
        return elapsedTime;
    }

    //wait for timer thread to finish
    ~Timer()
    {
        if (this->timerThread.joinable())
            this->timerThread.join();
    }
};

#endif
