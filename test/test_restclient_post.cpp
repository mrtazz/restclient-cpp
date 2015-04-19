#include "restclient-cpp/restclient.h"
#include <gtest/gtest.h>
#include <string>

class RestClientPostTest : public ::testing::Test
{
 protected:
    std::string url;
    std::string ctype;
    std::string data;

    RestClientPostTest()
    {
    }

    virtual ~RestClientPostTest()
    {
    }

    virtual void SetUp()
    {
      url = "http://http-test-server.herokuapp.com";
      ctype = "Content-Type: text/text";
      data = "data";
    }

    virtual void TearDown()
    {
    }

};

// Tests
TEST_F(RestClientPostTest, TestRestClientPOSTSimple)
{
  RestClient::response res = RestClient::post(url, ctype, data);
  EXPECT_EQ("POST", res.body);
}
// check return code
TEST_F(RestClientPostTest, TestRestClientPOSTCode)
{
  RestClient::response res = RestClient::post(url, ctype, data);
  EXPECT_EQ(200, res.code);
}
// check content type
TEST_F(RestClientPostTest, TestRestClientPOSTContentType)
{
  RestClient::response res = RestClient::post(url+"/contenttype",
                                              ctype, data);
  EXPECT_EQ(ctype, res.body);
}
// check response body
TEST_F(RestClientPostTest, TestRestClientPOSTBody)
{
  RestClient::response res = RestClient::post(url+"/body",
                                              ctype, data);
  EXPECT_EQ(data, res.body);
}
// check for failure
TEST_F(RestClientPostTest, TestRestClientFailureCode)
{
  std::string u = "http://nonexistent";
  RestClient::response res = RestClient::post(u, ctype, data);
  EXPECT_EQ(-1, res.code);
}
TEST_F(RestClientPostTest, TestRestClientPOSTHeaders)
{
  RestClient::response res = RestClient::post(url, ctype, data);
  EXPECT_EQ("keep-alive", res.headers["Connection"]);
}
