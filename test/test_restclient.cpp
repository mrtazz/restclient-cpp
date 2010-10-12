#include "restclient.h"
#include <gtest/gtest.h>
#include <string>

class RestClientTest : public ::testing::Test
{
 protected:
    int foo;
    std::string url;
    std::string ctype;
    std::string data;

    RestClientTest()
    {
    }

    virtual ~RestClientTest()
    {
    }

    virtual void SetUp()
    {
      foo = 0;
      url = "http://localhost:4567";
      ctype = "";
      data = "";
    }

    virtual void TearDown()
    {
    }

};

// Tests
TEST_F(RestClientTest, TestRestClientGETBody)
{
  RestClient::response res = RestClient::get(url);
  EXPECT_EQ("GET succesful.", res.body);
}
TEST_F(RestClientTest, TestRestClientPOST)
{
  RestClient::post(url, ctype, data);
  EXPECT_EQ(0, foo);
}
TEST_F(RestClientTest, TestRestClientPUT)
{
  RestClient::put(url, ctype, data);
  EXPECT_EQ(0, foo);
}
TEST_F(RestClientTest, TestRestClientDELETE)
{
  RestClient::del(url);
  EXPECT_EQ(0, foo);
}
