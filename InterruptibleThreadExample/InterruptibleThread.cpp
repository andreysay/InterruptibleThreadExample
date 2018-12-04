#include "InterruptibleThread.h"

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

template<typename FunctionType>
InterruptibleThread::InterruptibleThread(FunctionType f)
{
    std::promise<interrupt_flag*> p;
    internal_thread = std::thread([f, &p] {
            p.set_value(&this_thread_interrupt_flag);
            f();
        });
        flag = p.get_future().get();
}
void InterruptibleThread::interrupt()
{
    if (flag)
    {
        flag->set();
    }
}

