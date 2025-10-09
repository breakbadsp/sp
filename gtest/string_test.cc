#include <gtest/gtest.h>
#include "../sp/string.hpp"

// Basic constructor tests
TEST(StringTest, DefaultConstructor) {
  sp::string s;
  EXPECT_EQ(s.get_size(), 0);
  EXPECT_EQ(s.c_str(), nullptr);
}

TEST(StringTest, CStringConstructor) {
  const char* test = "Hello";
  sp::string s(test);
  EXPECT_EQ(s.get_size(), 5);
  EXPECT_STREQ(s.c_str(), "Hello");
}

TEST(StringTest, CharArrayConstructor) {
  char arr[] = "World";
  sp::string s(arr);
  EXPECT_EQ(s.get_size(), 5);
  EXPECT_STREQ(s.c_str(), "World");
}

TEST(StringTest, CopyConstructor) {
  sp::string s1("Test");
  sp::string s2(s1);
  EXPECT_EQ(s2.get_size(), 4);
  EXPECT_STREQ(s2.c_str(), "Test");
  // Ensure original is unchanged
  EXPECT_STREQ(s1.c_str(), "Test");
}

TEST(StringTest, MoveConstructor) {
  sp::string s1("Move");
  sp::string s2(std::move(s1));
  EXPECT_EQ(s2.get_size(), 4);
  EXPECT_STREQ(s2.c_str(), "Move");
  // Ensure original is moved from
  EXPECT_EQ(s1.get_size(), 0);
  EXPECT_EQ(s1.c_str(), nullptr);
}

// Assignment operator tests
TEST(StringTest, CopyAssignment) {
  sp::string s1("Original");
  sp::string s2;
  s2 = s1;
  EXPECT_EQ(s2.get_size(), 8);
  EXPECT_STREQ(s2.c_str(), "Original");
  // Ensure original is unchanged
  EXPECT_STREQ(s1.c_str(), "Original");
  
  // Self assignment, need to disable some compiler warnings for this -Wself-assign-overloaded
  //s1 = s1;
  //EXPECT_STREQ(s1.c_str(), "Original");
}

TEST(StringTest, MoveAssignment) {
  sp::string s1("Movable");
  sp::string s2;
  s2 = std::move(s1);
  EXPECT_EQ(s2.get_size(), 7);
  EXPECT_STREQ(s2.c_str(), "Movable");
  // Ensure original is moved from
  EXPECT_EQ(s1.get_size(), 0);
  EXPECT_EQ(s1.c_str(), nullptr);

  // Self move assignment, -Wself-move
  //sp::string s3("Self");
  //s3 = std::move(s3);
  //EXPECT_STREQ(s3.c_str(), "Self");
}

// Concatenation tests
TEST(StringTest, PlusOperator) {
  sp::string s1("Hello");
  sp::string s2(" World");
  sp::string s3 = s1 + s2;
  EXPECT_EQ(s3.get_size(), 11);
  EXPECT_STREQ(s3.c_str(), "Hello World");
  // Ensure originals are unchanged
  EXPECT_STREQ(s1.c_str(), "Hello");
  EXPECT_STREQ(s2.c_str(), " World");
}

TEST(StringTest, PlusEqualsOperator) {
  sp::string s1("Hello");
  sp::string s2(" World");
  s1 += s2;
  EXPECT_EQ(s1.get_size(), 11);
  EXPECT_STREQ(s1.c_str(), "Hello World");
  // Ensure second string is unchanged
  EXPECT_STREQ(s2.c_str(), " World");
  
  // Test empty string concatenation
  sp::string empty;
  s1 += empty;
  EXPECT_STREQ(s1.c_str(), "Hello World");
}

// Stream operator test
TEST(StringTest, StreamOperator) {
  sp::string s("StreamTest");
  std::ostringstream oss;
  oss << s;
  EXPECT_EQ(oss.str(), "StreamTest");
  
  // Test with empty string
  sp::string empty;
  std::ostringstream oss2;
  oss2 << empty;
  EXPECT_EQ(oss2.str(), "");
}