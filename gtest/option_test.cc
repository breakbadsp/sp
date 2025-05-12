#include <gtest/gtest.h>
#include <option.hpp>

class OptionTest : public testing::Test 
{
  public:
    OptionTest() = default;
};

TEST_F(OptionTest, ApiTestsWithInt) {
  sp::Option<int> o1;
  EXPECT_EQ(o1.HasValue(), false);
  EXPECT_EQ(o1.ValueOr(20), 20);

  int value = 10;
  sp::Option<int> o2(value);
  EXPECT_EQ(o2.HasValue(), true);
  EXPECT_EQ(o2.Value(), 10);
  EXPECT_EQ(o2.ValueOr(20), 10);
  EXPECT_EQ(*o2, 10);

  o2.Value() = 20;
  EXPECT_EQ(o2.Value(), 20);
  EXPECT_EQ(*o2, 20);
}

TEST_F(OptionTest, ApiTestsWithComplextStruct) {
  struct ComplexStruct {
    ComplexStruct() = default;
    ComplexStruct(const char* p_buff)
    {
      buff_ = new char[strlen(p_buff) + 1];
      strcpy(buff_, p_buff);
    }

    ComplexStruct(const ComplexStruct& p_rhs) 
    {
      buff_ = new char[strlen(p_rhs.buff_) + 1];
      strcpy(buff_, p_rhs.buff_);
    }

    ComplexStruct& operator=(const ComplexStruct& p_rhs) 
    {
      if(this != &p_rhs) {
        delete [] buff_;
        buff_ = new char[strlen(p_rhs.buff_) + 1];
        strcpy(buff_, p_rhs.buff_);
      }
      return *this;
    }

    ComplexStruct(ComplexStruct&& p_rhs) noexcept
    {
      buff_ = p_rhs.buff_;
      p_rhs.buff_ = nullptr;
    }

    ComplexStruct& operator=(ComplexStruct&& p_rhs) noexcept
    {
      if(this != &p_rhs) {
        delete [] buff_;
        buff_ = p_rhs.buff_;
        p_rhs.buff_ = nullptr;
      }
      return *this;
    }

    ~ComplexStruct() { delete [] buff_; buff_ = nullptr; }
    bool operator==(const ComplexStruct& p_rhs) const { return strcmp(buff_, p_rhs.buff_) == 0; } 
    bool operator!=(const ComplexStruct& p_rhs) const { return strcmp(buff_, p_rhs.buff_) != 0; } 
    const char* operator*() const { return buff_; }
    char* operator*() { return buff_; }

    char* buff_ {nullptr};
  };

  sp::Option<ComplexStruct> o1;
  ComplexStruct temp("Temp");
  EXPECT_EQ(o1.HasValue(), false);
  EXPECT_EQ(o1.ValueOr(temp), temp);

  ComplexStruct temp2("Temp2");
  sp::Option<ComplexStruct> o2(temp2);
  EXPECT_EQ(o2.HasValue(), true);
  EXPECT_EQ(o2.Value(), "Temp2");
  EXPECT_EQ(o2.ValueOr("Temp3"), "Temp2");
  EXPECT_EQ(*o2, "Temp2");

  o2.Value() = "Temp4";
  EXPECT_EQ(o2.Value(), "Temp4");
  EXPECT_EQ(*o2, "Temp4");
}