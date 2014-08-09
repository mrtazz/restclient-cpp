#include "restclient-cpp/restclient.h"
#include <gtest/gtest.h>
#include <string>

class RestClientGetTest : public ::testing::Test
{
 protected:
    std::string url;
    std::string ctype;
    std::string data;

    RestClientGetTest()
    {
    }

    virtual ~RestClientGetTest()
    {
    }

    virtual void SetUp()
    {
      url = "http://http-test-server.heroku.com";
      ctype = "";
      data = "";
    }

    virtual void TearDown()
    {
    }

};

// Tests
TEST_F(RestClientGetTest, TestRestClientGETBody)
{
  RestClient::response res = RestClient::get(url);
  EXPECT_EQ("GET", res.body);
}
// check return code
TEST_F(RestClientGetTest, TestRestClientGETCode)
{
  RestClient::response res = RestClient::get(url);
  EXPECT_EQ(200, res.code);
}
// check for failure
TEST_F(RestClientGetTest, TestRestClientFailureCode)
{
  std::string u = "http://nonexistent";
  RestClient::response res = RestClient::get(u);
  EXPECT_EQ(-1, res.code);
}

TEST_F(RestClientGetTest, TestRestClientGETHeaders)
{
  RestClient::response res = RestClient::get(url);
  EXPECT_EQ("keep-alive", res.headers["Connection"]);
}

