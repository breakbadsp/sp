#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "../sp/latch.hpp"


class LatchTest : public testing::Test 
{
  public:
    LatchTest() = default;

    void ArriveAndWait() 
    {
      count_waiting_++;
      l_.arrive_and_wait();
      count_waiting_--;
      count_released_++;
    }

    sp::latch l_ {3};
    std::atomic_int count_waiting_ {0};
    std::atomic_int count_released_ {0};
};


// Demonstrate some basic assertions.
TEST_F(LatchTest, BasicSingleThread) {
  sp::latch l(10); 
  EXPECT_EQ(l.try_wait(), false);
  l.count_down();//9
  EXPECT_EQ(l.try_wait(), false);
  l.count_down();//8
  EXPECT_EQ(l.try_wait(), false);
  l.count_down();//7
  EXPECT_EQ(l.try_wait(), false);
  l.count_down();//6
  EXPECT_EQ(l.try_wait(), false);
  l.count_down();//5
  EXPECT_EQ(l.try_wait(), false);
  l.count_down();//4
  EXPECT_EQ(l.try_wait(), false);
  l.count_down();//3
  EXPECT_EQ(l.try_wait(), false);
  l.count_down();//2
  EXPECT_EQ(l.try_wait(), false);
  l.count_down();//1
  EXPECT_EQ(l.try_wait(), false);
  l.count_down();//0
  EXPECT_EQ(l.try_wait(), true);
}

TEST_F(LatchTest, MultiThreaded) {
  std::vector<std::thread> threads; 

  EXPECT_EQ(l_.try_wait(), false);
  EXPECT_EQ(count_waiting_.load(), 0);
  EXPECT_EQ(count_released_.load(), 0);
  threads.emplace_back(&LatchTest::ArriveAndWait, this);//2
  
  EXPECT_EQ(l_.try_wait(), false);
  threads.emplace_back(&LatchTest::ArriveAndWait, this);//1
  
  EXPECT_EQ(l_.try_wait(), false);

  using namespace std::chrono_literals;
  std::this_thread::sleep_for(1s);
  EXPECT_EQ(count_waiting_.load(), 2);
  threads.emplace_back(&LatchTest::ArriveAndWait, this);//0

  for(auto& t: threads) {
    t.join();
  }
  EXPECT_EQ(l_.try_wait(), true);

  threads.clear();
  EXPECT_EQ(true, true); //we reached here
  EXPECT_EQ(count_waiting_.load(), 0);
  EXPECT_EQ(count_released_.load(), 3);
}

//TODO:: Its 3:40 am now, improve these tests later :)
