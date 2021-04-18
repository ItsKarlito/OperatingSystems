#pragma once

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

class thread_controller
{
public:
    enum status_t
    {
        RUNNING,
        IDLE,
        TERMINATED
    };

protected:
    // Pure virtual function that serves as the entry point for thread_controller's
    // children.
    virtual void cycle() = 0;

    // Creates the thread to be controlled (must be called before run/pause/terminate)                                                                           
    void create_thread();
    // Returns this->thread
    std::thread& get_thread();

public:
    // Changes the control flow for the thread
    void set_status(status_t status);
    // Gets the status of the thread
    status_t get_status() const;

    // Runs the thread
    void run();
    // Pauses the thread
    void pause();
    // Terminates the thread
    void terminate();

private:
    std::thread thread;
    std::mutex mtx;
    std::atomic<status_t> status;
    std::atomic<bool> alive;
    std::condition_variable cv;
};