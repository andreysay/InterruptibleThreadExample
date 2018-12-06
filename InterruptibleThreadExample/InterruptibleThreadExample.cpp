// InterruptibleThreadExample.cpp : Defines the entry point for the application.
//

#include "InterruptibleThreadExample.h"

std::atomic<unsigned int> counter;
std::vector<InterruptibleThread> backgroundThreads;
static unsigned int threadNum = 3;
std::mutex procMutex;

thread_local bool done = false;

void process_next_item()
{
    counter++;
    std::lock_guard<std::mutex> lock(procMutex);
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

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    for (size_t i = 0; i < threadNum; i++)
    {
        backgroundThreads[i].interrupt();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        backgroundThreads[i].join();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::cin.get();

    return 0;
}
