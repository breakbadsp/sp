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
  EXPECT_EQ(v11.size(), 0);
  EXPECT_EQ(v11.capacity(), 0);
  EXPECT_EQ(v11.data(), nullptr);
  EXPECT_TRUE(v11.empty());

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
  const sp::vector<int> v22(std::move(v21));
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

TEST(VectorTest, AssignmentOperator) {
  sp::vector<int> v11;
  sp::vector<int> v12;

  v12 = v11;
  EXPECT_EQ(v11.size(), 0);
  EXPECT_EQ(v11.capacity(), 0);
  EXPECT_EQ(v11.data(), nullptr);
  EXPECT_TRUE(v11.empty());

  EXPECT_EQ(v12.size(), 0);
  EXPECT_EQ(v12.capacity(), 0);
  EXPECT_EQ(v12.data(), nullptr);
  EXPECT_TRUE(v12.empty());

  sp::vector<int> v21(10);
  sp::vector<int> v22;

  v22 = v21;
  EXPECT_EQ(v22.size(), 0);
  EXPECT_EQ(v22.capacity(), 10);
  EXPECT_NE(v22.data(), nullptr);
  EXPECT_TRUE(v22.empty());

  EXPECT_EQ(v21.size(), 0);
  EXPECT_EQ(v21.capacity(), 10);
  EXPECT_NE(v21.data(), nullptr);
  EXPECT_TRUE(v21.empty());

  sp::vector<int> v23;
  v23 = std::move(v22);
  EXPECT_EQ(v22.size(), 0);
  EXPECT_EQ(v22.capacity(), 0);
  EXPECT_EQ(v22.data(), nullptr);
  EXPECT_TRUE(v22.empty());

  EXPECT_EQ(v23.size(), 0);
  EXPECT_EQ(v23.capacity(), 10);
  EXPECT_NE(v23.data(), nullptr);
  EXPECT_TRUE(v23.empty());
}

TEST(vector_test, ApiTests) {
  sp::vector<int> v1;
  v1.push_back(1);
  v1.push_back(2);
  int a = 10;
  v1.push_back(a);


  EXPECT_EQ(v1.back(), 10);
  EXPECT_EQ(v1[0], 1);
  EXPECT_EQ(v1[1], 2);
  EXPECT_EQ(v1[2], 10);

  const auto last = v1.pop_back();
  EXPECT_EQ(last, 10);

  //const back and [] operator
  const sp::vector<int> v2 = v1;
  EXPECT_EQ(v2.back(), 2);
  EXPECT_EQ(v2[0], 1);
  EXPECT_EQ(v2[1], 2);
}

TEST(vector_test, ApiTests2) {
  struct TestStruct {
    char* ptr = nullptr;
    TestStruct() {
      ptr = new char[20];
      strcpy(ptr, "TestStruct");
    }

    ~TestStruct() {
      delete [] ptr;
    }

    TestStruct(const char* p_rhs) {
      if (p_rhs) {
        ptr = new char[strlen(p_rhs) + 1];
        strcpy(ptr, p_rhs);
      }
    }

    TestStruct(const TestStruct& other) {
      if (other.ptr) {
        ptr = new char[strlen(other.ptr) + 1];
        strcpy(ptr, other.ptr);
      }
    }

    TestStruct(TestStruct&& other) noexcept :
      ptr(other.ptr)
    {
      other.ptr = nullptr;
    }

    TestStruct& operator=(const TestStruct& other) {
      if (this == &other)
          return *this;

      if (other.ptr) {
        delete [] ptr;
        ptr = new char[strlen(other.ptr) + 1];
        strcpy(ptr, other.ptr);
      }
      return *this;
    }

    TestStruct& operator=(TestStruct&& other) {
      if (this == &other) {
        return *this;
      }
      if (other.ptr) {
        ptr = other.ptr;
        other.ptr = nullptr;
      }
      return *this;
    }
  };

  sp::vector<TestStruct> v1;
  v1.push_back(TestStruct("TestStruct1"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  v1.push_back(TestStruct("TestStruct2"));
  TestStruct s1("TempStruct");
  v1.push_back(s1);
  v1.push_back(s1);
  v1.push_back(s1);
  v1.push_back(s1);

  sp::vector<TestStruct> v2 = v1;
  sp::vector<TestStruct> v3;
  v3 = v1;

  sp::vector<TestStruct> v4;
  v4 = std::move(v1);

  sp::vector<TestStruct> v5;
  TestStruct s2("TempStruct");
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
  v5.push_back(s2);
}



