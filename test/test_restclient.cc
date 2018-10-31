#include "restclient-cpp/restclient.h"
#include <gtest/gtest.h>
#include <json/json.h>
#include <string>

class RestClientTest : public ::testing::Test
{
 protected:

    RestClientTest()
    {
    }

    virtual ~RestClientTest()
    {
    }

    virtual void SetUp()
    {
      RestClient::init();
    }

    virtual void TearDown()
    {
      RestClient::disable();
    }

};

// DELETE Tests
// check return code
TEST_F(RestClientTest, TestRestClientDELETECode)
{
  RestClient::Response res = RestClient::del("http://httpbin.org/delete");
  EXPECT_EQ(200, res.code);
}

TEST_F(RestClientTest, TestRestClientDELETEBody)
{
  RestClient::Response res = RestClient::del("http://httpbin.org/delete");
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ("http://httpbin.org/delete", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" RESTCLIENT_VERSION, root["headers"].get("User-Agent", "no url set").asString());
}

// check for failure
TEST_F(RestClientTest, TestRestClientDELETEFailureCode)
{
  std::string u = "http://nonexistent";
  RestClient::Response res = RestClient::del(u);
  EXPECT_EQ(-1, res.code);
}

TEST_F(RestClientTest, TestRestClientDELETEHeaders)
{
  RestClient::Response res = RestClient::del("http://httpbin.org/delete");
  EXPECT_EQ("keep-alive", res.headers["Connection"]);
}

// GET Tests
TEST_F(RestClientTest, TestRestClientGETCode)
{
  RestClient::Response res = RestClient::get("http://httpbin.org/get");
  EXPECT_EQ(200, res.code);
}

TEST_F(RestClientTest, TestRestClientGETHTTP2Code)
{
  RestClient::Response res = RestClient::get("https://http2.golang.org/reqinfo");
  EXPECT_EQ(200, res.code);
}

TEST_F(RestClientTest, TestRestClientGETBodyCode)
{
  RestClient::Response res = RestClient::get("http://httpbin.org/get");
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ("http://httpbin.org/get", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" RESTCLIENT_VERSION, root["headers"].get("User-Agent", "no url set").asString());
}

// check for failure
TEST_F(RestClientTest, TestRestClientGETFailureCode)
{
  std::string u = "http://nonexistent";
  RestClient::Response res = RestClient::get(u);
  EXPECT_EQ("Failed to query.", res.body);
  EXPECT_EQ(-1, res.code);
}

TEST_F(RestClientTest, TestRestClientGETHeaders)
{
  RestClient::Response res = RestClient::get("http://httpbin.org/get");
  EXPECT_EQ("keep-alive", res.headers["Connection"]);
}

// POST Tests
// check return code
TEST_F(RestClientTest, TestRestClientPOSTCode)
{
  RestClient::Response res = RestClient::post("http://httpbin.org/post", "text/text", "data");
  EXPECT_EQ(200, res.code);
}

TEST_F(RestClientTest, TestRestClientPOSTBody)
{
  RestClient::Response res = RestClient::post("http://httpbin.org/post", "text/text", "data");
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ("http://httpbin.org/post", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" RESTCLIENT_VERSION, root["headers"].get("User-Agent", "no url set").asString());
}

// check for failure
TEST_F(RestClientTest, TestRestClientPOSTFailureCode)
{
  std::string u = "http://nonexistent";
  RestClient::Response res = RestClient::post(u, "text/text", "data");
  EXPECT_EQ(-1, res.code);
}

TEST_F(RestClientTest, TestRestClientPOSTHeaders)
{
  RestClient::Response res = RestClient::post("http://httpbin.org/post", "text/text", "data");
  EXPECT_EQ("keep-alive", res.headers["Connection"]);
}

// PUT Tests
// check return code
TEST_F(RestClientTest, TestRestClientPUTCode)
{
  RestClient::Response res = RestClient::put("http://httpbin.org/put", "text/text", "data");
  EXPECT_EQ(200, res.code);
}

TEST_F(RestClientTest, TestRestClientPUTBody)
{
  RestClient::Response res = RestClient::put("http://httpbin.org/put", "text/text", "data");
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ("http://httpbin.org/put", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" RESTCLIENT_VERSION, root["headers"].get("User-Agent", "no url set").asString());
}

// check for failure
TEST_F(RestClientTest, TestRestClientPUTFailureCode)
{
  std::string u = "http://nonexistent";
  RestClient::Response res = RestClient::put(u, "text/text", "data");
  EXPECT_EQ(-1, res.code);
}

TEST_F(RestClientTest, TestRestClientPUTHeaders)
{
  RestClient::Response res = RestClient::put("http://httpbin.org/put", "text/text", "data");
  EXPECT_EQ("keep-alive", res.headers["Connection"]);
}

// PATCH Tests
// check return code
TEST_F(RestClientTest, TestRestClientPATCHCode)
{
  RestClient::Response res = RestClient::patch("http://httpbin.org/patch", "text/text", "data");
  EXPECT_EQ(200, res.code);
}

TEST_F(RestClientTest, TestRestClientPATCHBody)
{
  RestClient::Response res = RestClient::patch("http://httpbin.org/patch", "text/text", "data");
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ("http://httpbin.org/patch", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" RESTCLIENT_VERSION, root["headers"].get("User-Agent", "no url set").asString());
}

// check for failure
TEST_F(RestClientTest, TestRestClientPATCHFailureCode)
{
  std::string u = "http://nonexistent";
  RestClient::Response res = RestClient::patch(u, "text/text", "data");
  EXPECT_EQ(-1, res.code);
}

TEST_F(RestClientTest, TestRestClientPATCHHeaders)
{
  RestClient::Response res = RestClient::patch("http://httpbin.org/put", "text/text", "data");
  EXPECT_EQ("keep-alive", res.headers["Connection"]);
}

// OPTIONS Tests
// check return code
// Disabled as httpbin does not support options requests for now
TEST_F(RestClientTest, DISABLED_TestRestClientOPTIONSCode)
{
  RestClient::Response res = RestClient::options("http://httpbin.org/options");
  EXPECT_EQ(200, res.code);
}

// check for failure
TEST_F(RestClientTest, TestRestClientOPTIONSFailureCode)
{
  std::string u = "http://nonexistent";
  RestClient::Response res = RestClient::options(u);
  EXPECT_EQ(-1, res.code);
}

TEST_F(RestClientTest, TestRestClientOPTIONSHeaders)
{
  RestClient::Response res = RestClient::options("http://httpbin.org/options");
  EXPECT_EQ("keep-alive", res.headers["Connection"]);
}

TEST_F(RestClientTest, TestRestClientAuth)
{
  RestClient::Response res = RestClient::get("http://foo:bar@httpbin.org/basic-auth/foo/bar");
  EXPECT_EQ(200, res.code);

  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ("foo", root.get("user", "no user").asString());
  EXPECT_EQ(true, root.get("authenticated", false).asBool());

  res = RestClient::get("http://httpbin.org/basic-auth/foo/bar");
  EXPECT_EQ(401, res.code);
}

TEST_F(RestClientTest, TestRestClientHeadCode)
{
  RestClient::Response res = RestClient::head("http://httpbin.org/get");
  EXPECT_EQ(200, res.code);
  EXPECT_EQ("", res.body);
}
