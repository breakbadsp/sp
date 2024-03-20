#pragma once

#include <thread>
#include <atomic>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>

namespace sp 
{
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

//copied from https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almost_equal(T x, T y, int ulp) noexcept
{
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() * std::fabs(x + y) * ulp
        // unless the result is subnormal
        || std::fabs(x - y) < std::numeric_limits<T>::min();
}

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    less_than(T x, T y, int ulp) noexcept
{
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    // if (x-y) is less than zero then return true, else false
    return ((x - y) < std::numeric_limits<T>::epsilon() * std::fabs(x + y) * ulp) && 
           !(almost_equal(x,y,ulp));
}

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    greater_than(T x, T y, int ulp) noexcept
{
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    // if (y-x) is less than zero then return true, else false
    return ((y - x) < std::numeric_limits<T>::epsilon() * std::fabs(x + y) * ulp) && 
          !(almost_equal(x,y,ulp));
}

template<class T>
constexpr std::remove_reference_t<T>&& move(T&& t) noexcept //t is universal ref or forwarding reference
{
  return static_cast<typename std::remove_reference<T>::type&&>(t);
}

template<class T>
constexpr T&& forward(T&& t) noexcept //t is universal ref or forwarding reference
{
  return static_cast<T&&>(t);
}

}//sp


