#ifndef SP_UTILITY_HPP
#define SP_UTILITY_HPP

#include <type_traits>

namespace sp
{

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

} // namespace sp

#endif //SP_UTILITY_HPP