#include "restclient-cpp/restclient.h"
#include "restclient-cpp/connection.h"
#include <gtest/gtest.h>
#include <json/json.h>
#include <string>

class ConnectionTest : public ::testing::Test
{
 protected:

    RestClient::Connection* conn;

    ConnectionTest()
    {
      conn = NULL;
    }

    virtual ~ConnectionTest()
    {
    }

    virtual void SetUp()
    {
      conn = new RestClient::Connection("http://httpbin.org");
    }

    virtual void TearDown()
    {
      delete conn;
    }

};

TEST_F(ConnectionTest, TestTimeout)
{
  std::string uri = "/delay/5";
  conn->SetTimeout(2);
  RestClient::Response res = conn->get(uri);
  EXPECT_EQ(28, res.code);
}

TEST_F(ConnectionTest, TestDefaultUserAgent)
{
  RestClient::Response res = conn->get("/get");
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ("http://httpbin.org/get", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" RESTCLIENT_VERSION,
      root["headers"].get("User-Agent", "nope/nope").asString());
}

TEST_F(ConnectionTest, TestCustomUserAgent)
{
  conn->SetUserAgent("foobar/1.2.3");
  RestClient::Response res = conn->get("/get");
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ("http://httpbin.org/get", root.get("url", "no url set").asString());
  EXPECT_EQ("foobar/1.2.3 restclient-cpp/" RESTCLIENT_VERSION,
      root["headers"].get("User-Agent", "nope/nope").asString());
}

TEST_F(ConnectionTest, TestBasicAuth)
{
  RestClient::Response res = conn->get("/basic-auth/foo/bar");
  EXPECT_EQ(401, res.code);

  conn->SetBasicAuth("foo", "bar");
  res = conn->get("/basic-auth/foo/bar");
  EXPECT_EQ(200, res.code);

  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ("foo", root.get("user", "no user").asString());
  EXPECT_EQ(true, root.get("authenticated", false).asBool());

}
