#pragma once

#include <type_traits>

#include "GblDefines.hpp"

namespace sp
{

  template <class T>
    requires std::is_integral_v<T>
  size_t Hash(T p_key, size_t table_size)
  {
    return p_key % table_size;
  };

  template <class T>
    requires std::is_bounded_array_v<T>
  size_t Hash(T p_key, size_t table_size)
  {
    size_t total = 0;
    for(const auto ch: p_key)
    {
      total += static_cast<size_t>(ch);
    }

    return total % table_size;
  };
}