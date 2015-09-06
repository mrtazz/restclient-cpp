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

  headers["Accept"] = "text/json";
  headers["Accept-Charset"] = "iso8859-2";
  headers["Accept-Language"] = "en-US";
  headers["User-Agent"] = "restclient-cpp";

  RestClient::response res = RestClient::get("http://httpbin.org/headers", headers);

  EXPECT_EQ(200, res.code);

  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  const Json::Value r_headers = root["headers"];
  for ( RestClient::headermap::const_iterator it = headers.begin(); it != headers.end(); ++it) {
	  EXPECT_EQ(it->second, r_headers.get(it->first, "Header " + it->first + " not found").asString());
  }
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

TEST_F(RestClientGetTest, TestRestClientGETTimeout)
{
  std::string u = "http://httpbin.org/delay/10";
  RestClient::response res = RestClient::get(u, 5);
  EXPECT_EQ(28, res.code);
}

TEST_F(RestClientGetTest, TestRestClientGETAuth)
{
  RestClient::setAuth("foo", "bar");
  RestClient::response res = RestClient::get("http://httpbin.org/basic-auth/foo/bar");
  EXPECT_EQ(200, res.code);

  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ("foo", root.get("user", "no user").asString());
  EXPECT_EQ(true, root.get("authenticated", false).asBool());

  RestClient::clearAuth();
  res = RestClient::get("http://httpbin.org/basic-auth/foo/bar");
  EXPECT_EQ(401, res.code);
}

