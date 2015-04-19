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
      url = "http://http-test-server.herokuapp.com";
      ctype = "";
      data = "";
    }

    virtual void TearDown()
    {
    }

};

// Tests
TEST_F(RestClientDeleteTest, TestRestClientDeleteBody)
{
  RestClient::response res = RestClient::del(url);
  EXPECT_EQ("DELETE", res.body);
}
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
