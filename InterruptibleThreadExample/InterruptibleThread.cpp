#include "InterruptibleThread.h"

extern void thread_interrupted();

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
        std::cout << "The thread with id: " << std::this_thread::get_id() << " has been interrupted!!!" << std::endl;
        throw thread_interrupted();
    }
}

template<typename FunctionType>
InterruptibleThread::InterruptibleThread(FunctionType f)
{
    std::promise<interrupt_flag*> p;
    internal_thread = std::thread([f, &p] {
            p.set_value(&this_thread_interrupt_flag);
            try
            {
                f();
            }
            catch (thread_interrupted const&)
            {

            }
        });
        flag = p.get_future().get();
}
void InterruptibleThread::join()
{
    internal_thread.join();
}
void InterruptibleThread::detach()
{
    internal_thread.detach();
}
bool InterruptibleThread::joinable() const
{
    return internal_thread.joinable();
}
void InterruptibleThread::interrupt()
{
    if (flag)
    {
        flag->set();
    }
}

