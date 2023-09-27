#pragma once

#include <thread>
#include <atomic>
#include <string>
#include <iostream>


inline auto SetThreadCore(int p_core_id)
{
  cpu_set_t cpu_set;
  CPU_ZERO(&cpu_set);
  CPU_SET(p_core_id, &cpu_set);
  return (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set), &cpu_set) == 0);
}

template <typename T, typename... A>
inline auto CreateAndRunThread(int p_core_id,
                               const std::string p_name,
                               T &&p_func,
                               A &&...p_args) noexcept
{
  std::atomic running(false), failed(false);
  auto thread_body = [&]
  {
    if (p_core_id >= 0 && !SetThreadCore(p_core_id))
    {
      failed = true;
      //log error
      return;
    }

    std::cout << "Starting thread with name " << p_name 
      << " on core " << p_core_id << '\n';
    //log thread name and core it is assigned to
    running = true;
    std::forward<T>(p_func)((std::forward<A>(p_args))...); //call function
  };

  auto t = new std::thread(thread_body);

  while(!failed && !running)
  {
    //using namesapce std::literals::chrono_literals;
    //std::this_thread::sleep_for(1s);
    sleep(1);
  }

  if(failed)
  {
    t->join();
    delete t;
    t = nullptr;
  }

  return t;
}

template <typename T, typename... A>
inline auto CreateAndRunThread(int p_core_id,
                               const std::string p_name,
                               T &&p_func) noexcept
{
  std::atomic running(false), failed(false);
  auto thread_body = [&]
  {
    if (p_core_id >= 0 && !SetThreadCore(p_core_id))
    {
      failed = true;
      //log error
      return;
    }

    std::cout << "Starting thread with name " << p_name 
      << " on core " << p_core_id << '\n';
    //log thread name and core it is assigned to
    running = true;
    std::forward<T>(p_func)(); //call function
  };

  auto t = new std::thread(thread_body);

  while(!failed && !running)
  {
    //using namesapce std::literals::chrono_literals;
    //std::this_thread::sleep_for(1s);
    sleep(1);
  }

  if(failed)
  {
    t->join();
    delete t;
    t = nullptr;
  }

  return t;
}
