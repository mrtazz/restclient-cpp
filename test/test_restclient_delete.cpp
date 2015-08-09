#include "restclient-cpp/restclient.h"
#include <gtest/gtest.h>
#include <string>

class RestClientDeleteTest : public ::testing::Test
{
 protected:
    std::string url;
    std::string ctype;
    std::string data;

    RestClientDeleteTest()
    {
    }

    virtual ~RestClientDeleteTest()
    {
    }

    virtual void SetUp()
    {
      url = "http://httpbin.org/delete";
      ctype = "";
      data = "";
    }

    virtual void TearDown()
    {
    }

};

// Tests
// check return code
TEST_F(RestClientDeleteTest, TestRestClientDeleteCode)
{
  RestClient::response res = RestClient::del(url);
  EXPECT_EQ(200, res.code);
}
// check for failure
TEST_F(RestClientDeleteTest, TestRestClientFailureCode)
{
  std::string u = "http://nonexistent";
  RestClient::response res = RestClient::del(u);
  EXPECT_EQ(-1, res.code);
}
TEST_F(RestClientDeleteTest, TestRestClientDeleteHeaders)
{
  RestClient::response res = RestClient::del(url);
  EXPECT_EQ("keep-alive", res.headers["Connection"]);
}
