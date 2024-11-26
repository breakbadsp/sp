#include <gtest/gtest.h>
#include "../sp/vector.hpp"


// Demonstrate some basic assertions.
TEST(VectorTest, Constructor) {
  sp::vector<int> v1;  
  EXPECT_EQ(v1.size(), 0);
  EXPECT_EQ(v1.capacity(), 0);
  EXPECT_EQ(v1.data(), nullptr);
  EXPECT_TRUE(v1.empty());
}

TEST(VectorTest, CapConstructor) {
  sp::vector<int> v1(10);
  EXPECT_EQ(v1.size(), 0);
  EXPECT_EQ(v1.capacity(), 10);
  EXPECT_NE(v1.data(), nullptr);
  EXPECT_TRUE(v1.empty());
}

TEST(VectorTest, CopyConstructor) {
  sp::vector<int> v11;
  sp::vector<int> v12(v11);
  EXPECT_EQ(v12.size(), 0);
  EXPECT_EQ(v12.capacity(), 0);
  EXPECT_EQ(v12.data(), nullptr);
  EXPECT_TRUE(v12.empty());

  sp::vector<int> v21(10);
  sp::vector<int> v22(v21);
  EXPECT_EQ(v22.size(), 0);
  EXPECT_EQ(v22.capacity(), 10);
  EXPECT_NE(v22.data(), nullptr);
  EXPECT_TRUE(v22.empty());
}

TEST(VectorTest, CopyConstructor2) {
  sp::vector<int> v11;
  sp::vector<int> v12(std::move(v11));
  EXPECT_EQ(v12.size(), 0);
  EXPECT_EQ(v12.capacity(), 0);
  EXPECT_EQ(v12.data(), nullptr);
  EXPECT_TRUE(v12.empty());
  
  EXPECT_EQ(v11.size(), 0);
  EXPECT_EQ(v11.capacity(), 0);
  EXPECT_EQ(v11.data(), nullptr);
  EXPECT_TRUE(v11.empty());

  sp::vector<int> v21(10);
  sp::vector<int> v22(std::move(v21));
  EXPECT_EQ(v22.size(), 0); //moved to
  EXPECT_EQ(v22.capacity(), 10);
  EXPECT_NE(v22.data(), nullptr);
  EXPECT_TRUE(v22.empty());
  EXPECT_EQ(v21.size(), 0); // moved from
  EXPECT_EQ(v21.capacity(), 0);
  EXPECT_EQ(v21.data(), nullptr);
  EXPECT_TRUE(v21.empty());

  //with some data
}


