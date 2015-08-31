#include "restclient-cpp/restclient.h"
#include "restclient-cpp/meta.h"
#include <gtest/gtest.h>
#include <json/json.h>
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
      url = "http://httpbin.org/get";
      ctype = "";
      data = "";
    }

    virtual void TearDown()
    {
    }

};

// Tests
// check return code
TEST_F(RestClientGetTest, TestRestClientGETCode)
{
  RestClient::response res = RestClient::get(url);
  EXPECT_EQ(200, res.code);
}
TEST_F(RestClientGetTest, TestRestClientGETBodyCode)
{
  RestClient::response res = RestClient::get(url);
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ("http://httpbin.org/get", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" VERSION, root["headers"].get("User-Agent", "no url set").asString());
}
//check if additional http headers were sent
TEST_F(RestClientGetTest, TestRestClientGETAdditionalHeaders)
{
  RestClient::headermap headers;
  headers["If-Modified-Since"] = "Sat, 29 Oct 1994 19:43:31 GMT";
  RestClient::response res = RestClient::get("http://httpbin.org/cache", headers);
  EXPECT_EQ(304, res.code);
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

