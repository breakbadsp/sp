#include <iostream>
#include <fstream>


#include "gtest/gtest.h"
#include "mmf.hpp"

class MmfTest : public testing::Test {
  public:
    MmfTest() = default;
};

TEST_F(MmfTest, ReadWriteTest) {
  sp::mmf mmf("testfile.mmf", 11, 0, O_RDWR);

  // Write some data
  const std::string_view data = "Hello, MMF!";
  mmf.Append(data);

  // Read the data back
  std::string_view read_data = mmf.ReadAllFrom(0);
  EXPECT_EQ(read_data.size(), data.size());
  EXPECT_EQ(std::string(read_data), data);
}

TEST_F(MmfTest, ReadWriteWithDelimiterTest) {
  sp::mmf mmf("testfile_delim.mmf", 20, 0, O_RDWR);

  // Write some data with a delimiter
  const std::string_view data = "Hello, MMF!";
  mmf.Append(data, '\n');

  // Read the data back
  std::string_view read_data = mmf.ReadAllFrom(0, '\n');
  EXPECT_EQ(read_data.size(), data.size());
  EXPECT_EQ(std::string(read_data), data);
}

TEST_F(MmfTest, ReadWriteWithOffsetTest) {
  sp::mmf mmf("testfile_offset1.mmf", 50, 0, O_RDWR);

  // Write some data
  const std::string_view data1 = "Hello, MMF!";
  mmf.Append(data1);

  // Write more data with an offset
  const std::string_view data2 = " More data.";
  mmf.Append(data2);

  // Read the data back
  std::string_view read_data1 = mmf.ReadFrom(0, data1.size());
  std::string_view read_data2 = mmf.ReadAllFrom(data1.size(), '\0');

  EXPECT_EQ(read_data1.size(), data1.size());
  EXPECT_EQ(std::string(read_data1), std::string(data1));
  EXPECT_EQ(read_data2.size(), data2.size());
  EXPECT_EQ(std::string(read_data2), data2);
}

TEST_F(MmfTest, ReadAnExistingFileWithOffsetTest) {
  std::string file_name = "testfile_offset2.mmf";
  std::ofstream ofs(file_name, std::ios::app);
  ASSERT_EQ(ofs.is_open(), true);

  const std::string d1 = "Hello, MMF1";
  const std::string d2 = "Hello, MMF2";
  const std::string d3 = "Hello, MMF3";

  auto ps = PAGE_SIZE * 2;
  while (ps > 0) {
    ofs << d1 << '\n';
    --ps;
  }

  ps = PAGE_SIZE * 2;
  while (ps > 0) {
    ofs << d2 << '\n';
    --ps;
  }

  ps = PAGE_SIZE * 2;
  while (ps > 0) {
    ofs << d3 << '\n';
    --ps;
  }

  //ofs.close();
  auto mmf1 = sp::mmf(file_name,
                      (PAGE_SIZE * 2),
                      0, // offset to the Hello MMF3
                      O_RDONLY);

  const std::string_view out(mmf1.ReadAllFrom(0, '\n'));
  EXPECT_EQ(out, d1);
}





























