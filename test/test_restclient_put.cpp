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
      url = "http://http-test-server.heroku.com";
      ctype = "Content-Type: text/text";
      data = "data";
    }

    virtual void TearDown()
    {
    }

};

// Tests
TEST_F(RestClientPutTest, TestRestClientPUTSimple)
{
  RestClient::response res = RestClient::put(url, ctype, data);
  EXPECT_EQ("PUT", res.body);
}
// check return code
TEST_F(RestClientPutTest, TestRestClientPUTCode)
{
  RestClient::response res = RestClient::put(url, ctype, data);
  EXPECT_EQ(200, res.code);
}
// check content type
TEST_F(RestClientPutTest, TestRestClientPUTContentType)
{
  RestClient::response res = RestClient::put(url+"/contenttype", ctype, data);
  EXPECT_EQ(ctype, res.body);
}
// check response body
TEST_F(RestClientPutTest, TestRestClientPUTBody)
{
  RestClient::response res = RestClient::put(url+"/body", ctype, data);
  EXPECT_EQ(data, res.body);
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
