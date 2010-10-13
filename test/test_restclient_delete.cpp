#include "restclient.h"
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
      url = "http://http-test-server.heroku.com";
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
