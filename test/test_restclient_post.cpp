#include "restclient-cpp/restclient.h"
#include "restclient-cpp/meta.h"
#include <gtest/gtest.h>
#include <json/json.h>
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
      url = "http://httpbin.org/post";
      ctype = "Content-Type: text/text";
      data = "data";
    }

    virtual void TearDown()
    {
    }

};

// Tests
// check return code
TEST_F(RestClientPostTest, TestRestClientPOSTCode)
{
  RestClient::response res = RestClient::post(url, ctype, data);
  EXPECT_EQ(200, res.code);
}
TEST_F(RestClientPostTest, TestRestClientPostBody)
{
  RestClient::response res = RestClient::post(url, ctype, data);
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ("http://httpbin.org/post", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" VERSION, root["headers"].get("User-Agent", "no url set").asString());
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

/*TEST_F(RestClientPostTest, TestRestClientPOSTTimeout)
{
  std::string u = "http://httpbin.org/delay/10";
  RestClient::response res = RestClient::post(u, ctype, data, 5);
  EXPECT_EQ(28, res.code);
}*/

