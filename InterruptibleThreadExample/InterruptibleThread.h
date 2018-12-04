#pragma once
#ifndef INTERRUPTIBLE_THREAD
#define INTERRUPTIBLE_THREAD

#include <atomic>
#include <thread>

class InterruptibleThread
{
public:
    template<typename FunctionType>
    InterruptibleThread(FunctionType f) = default;
    void join();
    void detach();
    bool joinable() const;
    void interrupt();
private:
    std::thread internal_thread;
    interrupt_flag* flag;
};


#endif // !INTERRUPTIBLE_THREAD

