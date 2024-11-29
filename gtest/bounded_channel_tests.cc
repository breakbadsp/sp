#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "../sp/bounded_channel.hpp"


class BoundedChannelTest : public testing::Test 
{
  public:
    BoundedChannelTest() = default;
};


// Demonstrate some basic assertions.
TEST_F(BoundedChannelTest, BasicSingleThread) {
  
  sp::bounded_channel<int> channel(4); 
  EXPECT_EQ(channel.size(), 0);
  EXPECT_EQ(channel.capacity(), 4);
  EXPECT_EQ(channel.empty(), true);
  EXPECT_EQ(channel.full(), false);
  
  channel.enqueue(0);
  EXPECT_EQ(channel.size(), 1);
  EXPECT_EQ(channel.capacity(), 4);
  EXPECT_EQ(channel.empty(), false);
  EXPECT_EQ(channel.full(), false);

  //Empty Test
  EXPECT_EQ(channel.dequeue(), 0);
  EXPECT_EQ(channel.size(), 0);
  EXPECT_EQ(channel.capacity(), 4);
  EXPECT_EQ(channel.empty(), true);
  EXPECT_EQ(channel.full(), false);

  //Full Test
  channel.enqueue(0);
  channel.enqueue(1);
  channel.enqueue(2);
  channel.enqueue(3);
  EXPECT_EQ(channel.size(), 4);
  EXPECT_EQ(channel.capacity(), 4);
  EXPECT_EQ(channel.empty(), false);
  EXPECT_EQ(channel.full(), true);

  //wrap around enqueue test
  //0
  EXPECT_EQ(channel.dequeue(), 0);
  EXPECT_EQ(channel.size(), 3);
  EXPECT_EQ(channel.capacity(), 4);
  EXPECT_EQ(channel.empty(), false);
  EXPECT_EQ(channel.full(), false);
  
  //wrap around enqueue
  channel.enqueue(4);
  EXPECT_EQ(channel.size(), 4);
  EXPECT_EQ(channel.capacity(), 4);
  EXPECT_EQ(channel.empty(), false);
  EXPECT_EQ(channel.full(), true);

  //wrap around dequeue test
  //1
  EXPECT_EQ(channel.dequeue(), 1);
  EXPECT_EQ(channel.size(), 3);
  EXPECT_EQ(channel.capacity(), 4);
  EXPECT_EQ(channel.empty(), false);
  EXPECT_EQ(channel.full(), false);

  //2
  EXPECT_EQ(channel.dequeue(), 2);
  EXPECT_EQ(channel.size(), 2);
  EXPECT_EQ(channel.capacity(), 4);
  EXPECT_EQ(channel.empty(), false);
  EXPECT_EQ(channel.full(), false);

  //3
  EXPECT_EQ(channel.dequeue(), 3);
  EXPECT_EQ(channel.size(), 1);
  EXPECT_EQ(channel.capacity(), 4);
  EXPECT_EQ(channel.empty(), false);
  EXPECT_EQ(channel.full(), false);

  //4
  EXPECT_EQ(channel.dequeue(), 4);
  EXPECT_EQ(channel.size(), 0);
  EXPECT_EQ(channel.capacity(), 4);
  EXPECT_EQ(channel.empty(), true);
  EXPECT_EQ(channel.full(), false);

}

/*
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
}*/

//TODO:: Its 3:40 am now, improve these tests later :)
