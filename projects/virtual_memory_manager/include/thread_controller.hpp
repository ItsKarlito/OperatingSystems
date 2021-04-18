#pragma once

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

class thread_controller
{
public:
    enum status_t                                                                                       // Used to define process status (shared by thread_controller)
    {
        RUNNING,
        IDLE,
        TERMINATED
    };

protected:
    virtual void cycle() = 0;                                                                       // Pure virtual function that serves as the entry point for thread_controller's
                                                                                                    // children
                                                                                                    
    void create_thread();                                                                           // Creates the thread to be controlled (must be called before run/pause/terminate)
    std::thread& get_thread();

public:
    void set_status(status_t status);                                                               // Changes the control flow for the thread
    status_t get_status() const;
    void run();
    void pause();
    void terminate();

private:
    std::thread thread;
    std::mutex mtx;
    std::atomic<status_t> status;
    std::atomic<bool> alive;
    std::condition_variable cv;
};