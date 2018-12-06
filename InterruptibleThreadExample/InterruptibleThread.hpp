#pragma once
#ifndef INTERRUPTIBLE_THREAD
#define INTERRUPTIBLE_THREAD

#include <atomic>
#include <thread>
#include <iostream>
#include <stdexcept>
#include <exception>
#include <future>

extern thread_local bool done;

class thread_interrupted : public std::exception
{
public:
    bool t_interrupted;
    thread_interrupted() : t_interrupted{ true } {}
};

namespace {
    class interrupt_flag
    {
    public:
        std::atomic<bool> flag;
        void set();
        bool is_set() const;
    };

    void interrupt_flag::set()
    {
        flag.store(true, std::memory_order_relaxed);
    }

    bool interrupt_flag::is_set() const
    {
        return flag.load(std::memory_order_relaxed);
    }

    thread_local interrupt_flag this_thread_interrupt_flag;

    void interruption_point()
    {
        if (this_thread_interrupt_flag.is_set())
        {
            std::cout << "The thread with id: " << std::this_thread::get_id() << " has been interrupted!!!\n" << std::flush;
            throw thread_interrupted();
        }
    }
}

class InterruptibleThread
{
public:
    template<typename FunctionType>
    InterruptibleThread(FunctionType f)
    {
        std::promise<interrupt_flag*> p;
        internal_thread = std::thread([f, &p] {
            p.set_value(&this_thread_interrupt_flag);
            try
            {
                f();
            }
            catch (thread_interrupted const& e)
            {
                done = e.t_interrupted;
            }
        });
        flag = p.get_future().get();
    }
    void join();
    void detach();
    bool joinable() const;
    void interrupt();
private:
    std::thread internal_thread;
    interrupt_flag* flag;
};


#endif // !INTERRUPTIBLE_THREAD

