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
      conn = new RestClient::Connection("https://httpbin.org");
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

TEST_F(ConnectionTest, TestFailForInvalidCA)
{
  // set a non-existing file for the CA file and it should fail to verify the peer
  conn->SetCAInfoFilePath("non-existent file");
  RestClient::Response res = conn->get("/get");

  EXPECT_EQ("Failed to query.", res.body);
  EXPECT_EQ(-1, res.code);
}

TEST_F(ConnectionTest, TestDefaultUserAgent)
{
  RestClient::Response res = conn->get("/get");
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ("https://httpbin.org/get", root.get("url", "no url set").asString());
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

  EXPECT_EQ("https://httpbin.org/get", root.get("url", "no url set").asString());
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

TEST_F(ConnectionTest, TestSetHeaders)
{
  RestClient::HeaderFields headers;
  headers["Foo"] = "bar";
  headers["Bla"] = "lol";
  conn->SetHeaders(headers);
  RestClient::Response res = conn->get("/headers");
  EXPECT_EQ(200, res.code);

  Json::Value root;
  std::istringstream str(res.body);
  str >> root;
  EXPECT_EQ("bar", root["headers"].get("Foo", "").asString());
  EXPECT_EQ("lol", root["headers"].get("Bla", "").asString());

  // let's replace the headers now
  RestClient::HeaderFields headers_again;
  headers_again["foo"] = "bob";
  conn->SetHeaders(headers_again);
  res = conn->get("/headers");
  EXPECT_EQ(200, res.code);

  std::istringstream str2(res.body);
  str2 >> root;
  EXPECT_EQ("bob", root["headers"].get("Foo", "").asString());
  // this shouldn't be set anymore
  EXPECT_EQ("", root["headers"].get("Bla", "").asString());
}

TEST_F(ConnectionTest, TestGetHeaders)
{
  RestClient::HeaderFields headers;
  headers["Foo"] = "bar";
  headers["Bla"] = "lol";
  conn->SetHeaders(headers);
  RestClient::Response res = conn->get("/headers");
  EXPECT_EQ(200, res.code);

  RestClient::HeaderFields headers_returned = conn->GetHeaders();
  EXPECT_EQ("bar", headers_returned["Foo"]);
  EXPECT_EQ("lol", headers_returned["Bla"]);

}

TEST_F(ConnectionTest, TestGetInfo)
{
  RestClient::HeaderFields headers;
  headers["Foo"] = "bar";
  headers["Bla"] = "lol";
  conn->SetHeaders(headers);
  conn->SetBasicAuth("foo", "bar");
  conn->SetTimeout(2);
  conn->SetUserAgent("foobar/1.2.3");
  conn->get("/get");
  RestClient::Connection::Info info = conn->GetInfo();
  EXPECT_EQ("bar", info.headers["Foo"]);
  EXPECT_EQ("lol", info.headers["Bla"]);
  EXPECT_EQ("foo", info.basicAuth.username);
  EXPECT_EQ("bar", info.basicAuth.password);
  EXPECT_EQ("foobar/1.2.3", info.customUserAgent);
  EXPECT_EQ(2, info.timeout);
  EXPECT_EQ("https://httpbin.org", info.baseUrl);
  EXPECT_NE(0, info.lastRequest.totalTime);
  EXPECT_NE(0, info.lastRequest.connectTime);
  EXPECT_NE(0, info.lastRequest.nameLookupTime);
  EXPECT_NE(0, info.lastRequest.appConnectTime);
  EXPECT_NE(0, info.lastRequest.preTransferTime);
  EXPECT_NE(0, info.lastRequest.startTransferTime);
  EXPECT_EQ(0, info.lastRequest.redirectTime);
  EXPECT_EQ(0, info.lastRequest.redirectCount);
}

TEST_F(ConnectionTest, TestFollowRedirect)
{
  RestClient::Response res = conn->get("/redirect/2");
  EXPECT_EQ(302, res.code);
  conn->FollowRedirects(true);
  res = conn->get("/redirect/2");
  EXPECT_EQ(200, res.code);
}

TEST_F(ConnectionTest, TestGetInfoFromRedirect)
{
  conn->FollowRedirects(true);
  RestClient::Response res = conn->get("/redirect/2");
  EXPECT_EQ(200, res.code);
  RestClient::Connection::Info info = conn->GetInfo();
  EXPECT_NE(0, info.lastRequest.redirectTime);
  EXPECT_NE(0, info.lastRequest.redirectCount);
}
