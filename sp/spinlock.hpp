#include <atomic>

namespace sp
{
  bool LOCKED = true;
  bool UNLOCKED = false;

  struct poor_mutex
  {
    int flag = 0;

    void lock()
    {
      while(flag != 0);
      flag = 1;
    }

    void unlock()
    {
      flag = 0; // no need to check the flag as it cannot be 1
    }
  };

  struct spinlock_v1
  {
    std::atomic_int flag = 0;

    void lock()
    {
      while(flag.load() != 0);
      flag.store(1);
    }

    void unlock()
    {
      flag.store(0); // no need to check the flag as it cannot be 1
    }
  };

  struct spinlock_v2
  {
    public:
      spinlock_v2() = default;
      spinlock_v2(const spinlock_v2&) = delete;
      spinlock_v2(spinlock_v2&&) = delete;
      spinlock_v2 operator=(const spinlock_v2&) = delete;
      spinlock_v2 operator=(spinlock_v2&&) = delete;

      void lock() const
      {
        while(!std::atomic_compare_exchange_weak(&is_locked, &UNLOCKED, LOCKED));
      }

      void unlock() const
      {
        //if unlock is called when it is already unlocked then behaviour is undefined.
        std::atomic_compare_exchange_strong(&is_locked, &LOCKED, UNLOCKED);
      }

    private:
      mutable std::atomic_bool is_locked = false;
  };
} // namespace sp