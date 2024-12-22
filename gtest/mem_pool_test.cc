#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <string>
#include "../sp/MemPool.hpp"


class MemPoolTest : public testing::Test 
{
  public:
    MemPoolTest() = default;
};

TEST_F(MemPoolTest, ConstructDestruct) {
  struct Test
  {
    Test() = default;
    ~Test() = default;
    int a;
    std::string str;
  };

  sp::MemPool<int> int_mem_pool(10);
  sp::MemPool<Test> struct_mem_pool(10);
  EXPECT_EQ(int_mem_pool.size(), 10);
  EXPECT_EQ(struct_mem_pool.size(), 10);
}
  
TEST_F(MemPoolTest, AllocationDeallocation) {
  struct Test
  {
    int a;
    std::string str;
  };

  sp::MemPool<int> int_mem_pool(10);
  sp::MemPool<Test> struct_mem_pool(10);

  auto* int_ptr = int_mem_pool.Allocate(1);
  EXPECT_NE(int_ptr, nullptr);
  *int_ptr = 10;
  EXPECT_EQ(*int_ptr, 10);
  int_mem_pool.Deallocate(int_ptr);

  auto* str = struct_mem_pool.Allocate();
  EXPECT_NE(str, nullptr);
  str->a = 10;
  auto temp = std::string("test string");
  str->str = temp;
  EXPECT_EQ(str->a, 10);
  EXPECT_EQ(str->str, temp);
  struct_mem_pool.Deallocate(str);

}

TEST_F(MemPoolTest, Relloaction) {
  struct Test
  {
    int a;
    std::string str;
  };

  sp::MemPool<int> int_mem_pool(11);
  sp::MemPool<Test> struct_mem_pool(11);

  auto* int_ptr = int_mem_pool.Allocate(1);
  EXPECT_NE(int_ptr, nullptr);
  *int_ptr = 10;
  EXPECT_EQ(*int_ptr, 10);
  int_mem_pool.Deallocate(int_ptr);

  auto* str = struct_mem_pool.Allocate();
  EXPECT_NE(str, nullptr);
  str->a = 10;
  auto temp = std::string("test string");
  str->str = temp;
  EXPECT_EQ(str->a, 10);
  EXPECT_EQ(str->str, temp);
  struct_mem_pool.Deallocate(str);

  int_ptr = int_mem_pool.Allocate(1);
  EXPECT_NE(int_ptr, nullptr);
  *int_ptr = 10;
  EXPECT_EQ(*int_ptr, 10);
  int_mem_pool.Deallocate(int_ptr);

  str = struct_mem_pool.Allocate();
  EXPECT_NE(str, nullptr);
  str->a = 10;
  temp = std::string("test string2");
  str->str = temp;
  EXPECT_EQ(str->a, 10);
  EXPECT_EQ(str->str, temp);
  struct_mem_pool.Deallocate(str);

}

TEST_F(MemPoolTest, RelloactionLoop) {
  struct Test
  {
    int a;
    std::string str;
  };

  sp::MemPool<Test> struct_mem_pool(11);

  std::vector<int*> vec1;
  std::vector<Test*> vec2;
  for(int i = 0; i < 10; ++i)
  {
    auto* str = struct_mem_pool.Allocate();
    EXPECT_NE(str, nullptr);
    str->a = 10;
    auto temp = std::string("test string");
    str->str = temp;
    EXPECT_EQ(str->a, 10);
    EXPECT_EQ(str->str, temp);
    vec2.push_back(str);
  }

  for(int i = 0; i < 10; ++i)
  {
    struct_mem_pool.Deallocate(vec2[i]);
  }

  vec2.clear();
  for(int i = 0; i < 10; ++i)
  {
    auto* str = struct_mem_pool.Allocate();
    EXPECT_NE(str, nullptr);
    str->a = 10;
    auto temp = std::string("test string");
    str->str = temp;
    EXPECT_EQ(str->a, 10);
    EXPECT_EQ(str->str, temp);
    vec2.push_back(str);
  }

  for(int i = 0; i < 10; ++i)
  {
    struct_mem_pool.Deallocate(vec2[i]);
  }

}