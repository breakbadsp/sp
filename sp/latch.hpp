#include <atomic>
#include <mutex>
#include <condition_variable>

namespace sp {
class latch {
public:
  latch() = delete;
  latch(int p_count ) 
  : count(p_count)
  , latched(false)
  {}

  void decreament_count() {
    --count;
    if(count == 0)
    {
      latched = true;
      cv.notify_all();
    }
  }

  void wait() {
    std::unique_lock<std::mutex> ul(lock);
    while(!latched) {
      cv.wait(ul);
    }
  }

private:
  std::atomic_int count = {0};
  std::atomic_bool latched = {false};
  std::condition_variable cv;
  std::mutex lock;
};
}//sp