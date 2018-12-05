#pragma once
#ifndef INTERRUPTIBLE_THREAD
#define INTERRUPTIBLE_THREAD

#include <atomic>
#include <thread>
#include <iostream>

class interrupt_flag;

class InterruptibleThread
{
public:
    template<typename FunctionType>
    InterruptibleThread(FunctionType f);
    void join();
    void detach();
    bool joinable() const;
    void interrupt();
private:
    std::thread internal_thread;
    interrupt_flag* flag;
};


#endif // !INTERRUPTIBLE_THREAD

