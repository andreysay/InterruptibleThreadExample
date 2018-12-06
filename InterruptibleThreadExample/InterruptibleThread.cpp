#include "InterruptibleThread.hpp"

//extern void thread_interrupted();

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

