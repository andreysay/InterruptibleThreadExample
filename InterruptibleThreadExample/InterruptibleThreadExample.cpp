// InterruptibleThreadExample.cpp : Defines the entry point for the application.
//

#include "InterruptibleThreadExample.h"

std::atomic<unsigned int> counter;
std::vector<InterruptibleThread> backgroundThreads;
unsigned long max_threads = 4;
unsigned long const hardware_threads = std::thread::hardware_concurrency();
unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
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
    for (size_t i = 0; i < max_threads; i++)
    {
        backgroundThreads.push_back(InterruptibleThread(foo));
    }
}

int main()
{
    std::cout << "Max threads on this computer is: " << max_threads << std::endl;
    std::cout << "Hardware threads detected: " << hardware_threads << std::endl;
    start_background_processing();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    for (size_t i = 0; i < max_threads; i++)
    {
        backgroundThreads[i].interrupt();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        backgroundThreads[i].join();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::cin.get();

    return 0;
}
