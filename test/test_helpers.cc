#include "restclient-cpp/helpers.h"
#include <gtest/gtest.h>
#include <string>

class HelpersTest : public ::testing::Test
{
 protected:

    HelpersTest()
    {
    }

    virtual ~HelpersTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(HelpersTest, TrimLeft) {
  auto can_trim_left = std::string("    a set of characters");
  auto cant_trim_left = std::string("a set of characters");
  auto right_trim_ignored = std::string("a set of characters    ");

  EXPECT_EQ(RestClient::Helpers::ltrim(can_trim_left),
            std::string("a set of characters"));
  EXPECT_EQ(RestClient::Helpers::ltrim(cant_trim_left),
            std::string("a set of characters"));
  EXPECT_EQ(RestClient::Helpers::ltrim(right_trim_ignored),
            std::string("a set of characters    "));
}

TEST_F(HelpersTest, TrimRight) {
  auto left_trim_ignored = std::string("    a set of characters");
  auto cant_trim_right = std::string("a set of characters");
  auto can_trim_right = std::string("a set of characters    ");

  EXPECT_EQ(RestClient::Helpers::rtrim(left_trim_ignored),
            std::string("    a set of characters"));
  EXPECT_EQ(RestClient::Helpers::rtrim(cant_trim_right),
            std::string("a set of characters"));
  EXPECT_EQ(RestClient::Helpers::rtrim(can_trim_right),
            std::string("a set of characters"));
}

TEST_F(HelpersTest, TrimBoth) {
  auto can_trim_both = std::string("    a set of characters    ");
  auto can_trim_left = std::string("    a set of characters");
  auto can_trim_right = std::string("a set of characters    ");
  auto cant_trim = std::string("a set of characters");

  EXPECT_EQ(RestClient::Helpers::trim(can_trim_both), "a set of characters");
  EXPECT_EQ(RestClient::Helpers::trim(can_trim_left), "a set of characters");
  EXPECT_EQ(RestClient::Helpers::trim(can_trim_right), "a set of characters");
  EXPECT_EQ(RestClient::Helpers::trim(cant_trim), "a set of characters");
}
