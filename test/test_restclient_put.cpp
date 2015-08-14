#include "restclient-cpp/restclient.h"
#include "restclient-cpp/meta.h"
#include <gtest/gtest.h>
#include <json/json.h>
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
TEST_F(RestClientPutTest, TestRestClientPutBody)
{
  RestClient::response res = RestClient::put(url, ctype, data);
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ("http://httpbin.org/put", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" VERSION, root["headers"].get("User-Agent", "no url set").asString());
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
