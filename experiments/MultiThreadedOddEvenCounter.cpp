#include <thread>
#include <iostream>
#include <mutex>

int main()
{
    uint32_t counter = 1;
    std::mutex counter_mutex;

    std::thread odd_number_printer([&counter_mutex, &counter]{
        while(true)
        {
            sched_yield();
            sched_yield();
            sched_yield();
            sched_yield();
            sched_yield();
            sched_yield();
            sched_yield();
            sched_yield();
            sched_yield();
            //sleep(3);
            {
                std::lock_guard lg(counter_mutex);
                if(counter < 101 && (counter % 2) != 0)
                {
                    std::cout << counter << ' ';
                    ++counter;
                }
                if(counter >= 100)
                    break;
            }
        }
    });

    std::thread even_number_printer([&counter_mutex, &counter]{
        while(true)
        {
            {
                std::lock_guard lg(counter_mutex);
                if(counter < 101 && (counter % 2) == 0) //even
                {
                    std::cout << counter << ' ';
                    ++counter;
                }
                if(counter >= 100)
                    break;
            }
        }
    });

    even_number_printer.join();
    odd_number_printer.join();
};