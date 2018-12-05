// InterruptibleThreadExample.cpp : Defines the entry point for the application.
//

#include "InterruptibleThreadExample.h"

std::atomic<unsigned int> counter;
std::vector<InterruptibleThread> backgroundThreads;
static unsigned int threadNum = 10;

thread_local bool done = false;

extern void interruption_point();

void thread_interrupted()
{
    done = true;
}

void process_next_item()
{
    counter++;
    std::cout << "The thread with id: " << std::this_thread::get_id() << " has counter: " << counter << std::endl;
}

void foo()
{

    while (!done)
    {
        interruption_point();
        process_next_item();
    }
}

void start_background_processing()
{
    for (size_t i = 0; i < threadNum; i++)
    {
        backgroundThreads.push_back(InterruptibleThread(foo));
    }
}

int main()
{
    start_background_processing();

    for (size_t i = 0; i < threadNum; i++)
    {
        backgroundThreads[i].interrupt();
    }

    for (size_t i = 0; i < threadNum; i++)
    {
        backgroundThreads[i].join();
    }

	return 0;
}
