#include "restclient-cpp/restclient.h"
#include <gtest/gtest.h>
#include <string>

class RestClientPutTest : public ::testing::Test
{
 protected:
    std::string url;
    std::string ctype;
    std::string data;

    RestClientPutTest()
    {
    }

    virtual ~RestClientPutTest()
    {
    }

    virtual void SetUp()
    {
      url = "http://httpbin.org/put";
      ctype = "Content-Type: text/text";
      data = "data";
    }

    virtual void TearDown()
    {
    }

};

// Tests
// check return code
TEST_F(RestClientPutTest, TestRestClientPUTCode)
{
  RestClient::response res = RestClient::put(url, ctype, data);
  EXPECT_EQ(200, res.code);
}
// check for failure
TEST_F(RestClientPutTest, TestRestClientFailureCode)
{
  std::string u = "http://nonexistent";
  RestClient::response res = RestClient::put(u, ctype, data);
  EXPECT_EQ(-1, res.code);
}
TEST_F(RestClientPutTest, TestRestClientPUTHeaders)
{
  RestClient::response res = RestClient::put(url, ctype, data);
  EXPECT_EQ("keep-alive", res.headers["Connection"]);
}
