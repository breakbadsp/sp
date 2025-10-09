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

// Comparison operator tests
TEST(StringTest, ComparisonOperators) {
  sp::string s1("Apple");
  sp::string s2("Banana");
  sp::string s3("Apple"); // Same as s1

  EXPECT_TRUE(s1 == s3);
  EXPECT_FALSE(s1 == s2);
  EXPECT_TRUE(s1 != s2);
  EXPECT_TRUE(s1 < s2);
  EXPECT_TRUE(s2 > s1);
  EXPECT_TRUE(s1 <= s3);
  EXPECT_TRUE(s2 >= s2);
  EXPECT_FALSE(s2 < s1);
  EXPECT_FALSE(s1 > s2);
  EXPECT_TRUE(s1 <= s2);
  EXPECT_TRUE(s2 >= s1);
  EXPECT_FALSE(s1 != s3);
  EXPECT_FALSE(s1 < s3);
  EXPECT_FALSE(s1 > s3);
  EXPECT_TRUE(s1 <= s3);
  EXPECT_TRUE(s1 >= s3);
}

//null, empty check testing if nullptr passed then exception etc for comparison operators
TEST(StringTest, ComparisonWithNullAndEmpty) {
  sp::string s1; // empty string
  sp::string s2("NonEmpty");

  EXPECT_TRUE(s1 == s1); // empty == empty
  EXPECT_FALSE(s1 == s2); // empty != non-empty
  EXPECT_TRUE(s1 != s2);
  EXPECT_TRUE(s1 < s2);
  EXPECT_FALSE(s2 < s1);
  EXPECT_TRUE(s1 <= s2);
  EXPECT_FALSE(s2 <= s1);
  EXPECT_FALSE(s1 > s2);
  EXPECT_TRUE(s2 > s1);
  EXPECT_FALSE(s1 >= s2);
  EXPECT_TRUE(s2 >= s1);

  sp::string s3; // null string treated as empty
  EXPECT_TRUE(s1 == s3);
  EXPECT_FALSE(s1 != s3);
  EXPECT_FALSE(s1 < s3);
  EXPECT_FALSE(s3 < s1);
  EXPECT_TRUE(s1 <= s3);
  EXPECT_TRUE(s3 <= s1);
  EXPECT_FALSE(s1 > s3);
  EXPECT_FALSE(s3 > s1);
  EXPECT_TRUE(s1 >= s3);
  EXPECT_TRUE(s3 >= s1);
}

// Substring tests
TEST(StringTest, SubstrMethod) {
  sp::string s("SubstringTest");
  sp::string sub1 = s.substr(0, 9); // "Substring"
  EXPECT_EQ(sub1.get_size(), 9);
  EXPECT_STREQ(sub1.c_str(), "Substring");
}
TEST(StringTest, SubstrOutOfRange) {
  sp::string s("Short");
  EXPECT_THROW(s.substr(10, 5), std::out_of_range);
}

TEST(StringTest, SubstrLengthExceeds) {
  sp::string s("Exact");
  sp::string sub = s.substr(2, 10); // Should return "act"
  EXPECT_EQ(sub.get_size(), 3);
  EXPECT_STREQ(sub.c_str(), "act");
}
//sub from 0 to npos
TEST(StringTest, SubstrToNpos) {
  sp::string s("FullString");
  sp::string sub = s.substr(0, sp::string::npos); // Should return "FullString"
  EXPECT_EQ(sub.get_size(), 10);
  EXPECT_STREQ(sub.c_str(), "FullString");
}

//sub from 0 to middle randome
TEST(StringTest, SubstrFromZeroToMiddle) {
  sp::string s("MiddlePart");
  sp::string sub = s.substr(0, 6); // Should return "Middle"
  EXPECT_EQ(sub.get_size(), 6);
  EXPECT_STREQ(sub.c_str(), "Middle");
}

//sub from 0 to size but by passing bigger than size
TEST(StringTest, SubstrFromZeroToBiggerThanSize) {
  sp::string s("SizeTest");
  sp::string sub = s.substr(0, 20); // Should return "SizeTest"
  EXPECT_EQ(sub.get_size(), 8);
  EXPECT_STREQ(sub.c_str(), "SizeTest");
}

// sun from middle to npos
TEST(StringTest, SubstrFromMiddleToNpos) {
  sp::string s("MiddleToEnd");
  sp::string sub = s.substr(6, sp::string::npos); // Should return "ToEnd"
  EXPECT_EQ(sub.get_size(), 5);
  EXPECT_STREQ(sub.c_str(), "ToEnd");
}

//sub from middle to size but by passing bigger than size
TEST(StringTest, SubstrFromMiddleToBiggerThanSize) {
  sp::string s("MiddleToSize");
  sp::string sub = s.substr(6, 50); // Should return "ToSize"
  EXPECT_EQ(sub.get_size(), 6);
  EXPECT_STREQ(sub.c_str(), "ToSize");
}

//sub from middle to middle
TEST(StringTest, SubstrFromMiddleToMiddle) {
  sp::string s("MiddlePartTest");
  sp::string sub = s.substr(6, 4); // Should return "Part"
  EXPECT_EQ(sub.get_size(), 4);
  EXPECT_STREQ(sub.c_str(), "Part");
}

//last char
TEST(StringTest, SubstrLastChar) {
  sp::string s("LastChar");
  sp::string sub = s.substr(7, 1); // Should return "r"
  EXPECT_EQ(sub.get_size(), 1);
  EXPECT_STREQ(sub.c_str(), "r");
}

// sub first char
TEST(StringTest, SubstrFirstChar) {
  sp::string s("FirstChar");
  sp::string sub = s.substr(0, 1); // Should return "F"
  EXPECT_EQ(sub.get_size(), 1);
  EXPECT_STREQ(sub.c_str(), "F");
}


// Find method tests

// Find existing substring
TEST(StringTest, FindMethod) {
  sp::string s("FindMethodTest");
  EXPECT_EQ(s.find("Method"), 4);
  EXPECT_EQ(s.find("Test"), 10);
  EXPECT_EQ(s.find("NotFound"), sp::string::npos);
}

// Find with position
TEST(StringTest, FindWithPosition) {
  sp::string s("FindMethodTestMethod");
  EXPECT_EQ(s.find("Method", 5), 14);
  EXPECT_EQ(s.find("Method", 0), 4);
  EXPECT_EQ(s.find("Method", 16), sp::string::npos);
}

// Find empty string
TEST(StringTest, FindEmptyString) {
  sp::string s("EmptyFind");
  EXPECT_EQ(s.find(""), sp::string::npos);
}

// Find substring larger than string
TEST(StringTest, FindLargerSubstring) {
  sp::string s("Short");
  EXPECT_EQ(s.find("ThisIsLonger"), sp::string::npos);
}

// Find with position out of range
TEST(StringTest, FindPositionOutOfRange) {
  sp::string s("OutOfRange");
  EXPECT_EQ(s.find("Range", 20), sp::string::npos);
}

// Empty method tests
TEST(StringTest, IsEmptyMethod) {
  sp::string s1;
  EXPECT_TRUE(s1.is_empty());
}

TEST(StringTest, IsNotEmptyMethod) {
  sp::string s2("NotEmpty");
  EXPECT_FALSE(s2.is_empty());
}

//clear method tests
TEST(StringTest, ClearMethod) {
  sp::string s("ToBeCleared");
  s.clear();
  EXPECT_TRUE(s.is_empty());
  EXPECT_EQ(s.get_size(), 0);
  EXPECT_EQ(s.c_str(), nullptr);
}

TEST(StringTest, ClearEmptyString) {
  sp::string s;
  s.clear();
  EXPECT_TRUE(s.is_empty());
  EXPECT_EQ(s.get_size(), 0);
  EXPECT_EQ(s.c_str(), nullptr);
}

//size method tests
TEST(StringTest, SizeMethod) {
  sp::string s1;
  EXPECT_EQ(s1.get_size(), 0);
  sp::string s2("SizeCheck");
  EXPECT_EQ(s2.get_size(), 9);
  s2.clear();
  EXPECT_EQ(s2.get_size(), 0);
  s2 = sp::string("NewSize");
  EXPECT_EQ(s2.get_size(), 7);
}

//c_str method tests
TEST(StringTest, CStrMethod) {
  sp::string s1;
  EXPECT_EQ(s1.c_str(), nullptr);
  sp::string s2("CStrCheck");
  EXPECT_STREQ(s2.c_str(), "CStrCheck");
  s2.clear();
  EXPECT_EQ(s2.c_str(), nullptr);
  s2 = sp::string("AnotherCheck");
  EXPECT_STREQ(s2.c_str(), "AnotherCheck");
  EXPECT_EQ(s2.get_size(), 12);
}

// Swap function test
TEST(StringTest, SwapFunction) {
  sp::string s1("First");
  sp::string s2("Second");
  swap(s1, s2);
  EXPECT_STREQ(s1.c_str(), "Second");
  EXPECT_STREQ(s2.c_str(), "First");
  EXPECT_EQ(s1.get_size(), 6);
  EXPECT_EQ(s2.get_size(), 5);
}