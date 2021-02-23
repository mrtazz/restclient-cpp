#include "restclient-cpp/restclient.h"
#include "restclient-cpp/connection.h"
#include <gtest/gtest.h>
#include <json/json.h>
#include <string>

#include "tests.h"

class ConnectionTest : public ::testing::Test
{
 protected:

    RestClient::Connection* conn;

    ConnectionTest()
    {
      conn = NULL;
      testUrl = RestClient::TestUrl;
    }

    virtual ~ConnectionTest()
    {
    }

    virtual void SetUp()
    {
      // Port below should match what is set in Makefile.in
      conn = new RestClient::Connection(testUrl);
      conn->SetTimeout(10);
    }

    virtual void TearDown()
    {
      delete conn;
    }

    std::string testUrl;
};

class ConnectionTestRemote : public ConnectionTest
{
protected:
    ConnectionTestRemote()
    {
      // Port below should match what is set in Makefile.in
      testUrl = "https://httpbin.org";
    }
};

TEST_F(ConnectionTest, TestTimeout)
{
  std::string uri = "/delay/5";
  conn->SetTimeout(4);
  RestClient::Response res = conn->get(uri);
  EXPECT_EQ(28, res.code);
}

TEST_F(ConnectionTestRemote, TestFailForInvalidCA)
{
  // set a non-existing file for the CA file and it should fail to verify the peer
  conn->SetCAInfoFilePath("non-existent file");
  RestClient::Response res = conn->get("/get");

  EXPECT_EQ("Problem with the SSL CA cert (path? access rights?)", res.body);
  EXPECT_EQ(77, res.code);
}

TEST_F(ConnectionTestRemote, TestAllowInsecure)
{
  // set a non-existing file for the CA file, should allow access anyway
  conn->SetCAInfoFilePath("non-existent file");
  conn->SetVerifyPeer(false);
  RestClient::Response res = conn->get("/get");

  EXPECT_EQ(200, res.code);
}

TEST_F(ConnectionTest, TestDefaultUserAgent)
{
  RestClient::Response res = conn->get("/get");
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ(testUrl + "/get", root.get("url", "no url set").asString());
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

  EXPECT_EQ(testUrl + "/get", root.get("url", "no url set").asString());
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

// test below can succeed. should run https server locally to control expected behavior.
// TEST_F(ConnectionTestRemote, TestSSLCert)
// {
//   conn->SetCertPath("non-existent file");
//   conn->SetKeyPath("non-existent key path");
//   conn->SetKeyPassword("imaginary_password");
//   conn->SetCertType("invalid cert type");
//   RestClient::Response res = conn->get("/get");
// 
//   EXPECT_EQ(58, res.code);
// }

TEST_F(ConnectionTest, TestCurlError)
{
	auto cancelCallback = [](void* pData, double downloadTotal, double downloaded, double uploadTotal, double uploaded) -> int {
    // abort connection at first progress callback
    return 1;
	};
	conn->SetFileProgressCallback(cancelCallback);
	conn->SetFileProgressCallbackData(NULL);

  RestClient::Response res = conn->get("/get");
  int errorCode = conn->GetInfo().lastRequest.curlCode;

  EXPECT_EQ(42, res.code);
  EXPECT_EQ(42, errorCode);
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

TEST_F(ConnectionTestRemote, TestGetInfo)
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
  EXPECT_EQ(testUrl, info.baseUrl);
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

TEST_F(ConnectionTest, TestFollowRedirectLimited)
{
  RestClient::Response res = conn->get("/redirect/2");
  EXPECT_EQ(302, res.code);
  conn->FollowRedirects(true, 1);
  res = conn->get("/redirect/2");
  // 47 = CURLE_TOO_MANY_REDIRECTS 
  EXPECT_EQ(47, res.code);
  conn->FollowRedirects(true, 2);
  res = conn->get("/redirect/2");
  EXPECT_EQ(200, res.code);
  conn->FollowRedirects(true, -1);
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

TEST_F(ConnectionTest, TestHeadHeaders)
{
  RestClient::HeaderFields headers;
  headers["Foo"] = "bar";
  headers["Bla"] = "lol";
  conn->SetHeaders(headers);
  RestClient::Response res = conn->head("/headers");
  EXPECT_EQ(200, res.code);
  EXPECT_EQ("", res.body);

  RestClient::HeaderFields headers_returned = conn->GetHeaders();
  EXPECT_EQ("bar", headers_returned["Foo"]);
  EXPECT_EQ("lol", headers_returned["Bla"]);
}

TEST_F(ConnectionTest, TestNoSignal)
{
  conn->SetNoSignal(true);
  RestClient::Response res = conn->get("/get");
  EXPECT_EQ(200, res.code);
}

TEST_F(ConnectionTest, TestSetProgress)
{
	static double totalToDownload = 0;
	static double totalDownloaded = 0;

	auto progressCallback = [](void* pData, double downloadTotal, double downloaded, double uploadTotal, double uploaded) -> int {
    totalToDownload = downloadTotal;
    totalDownloaded = downloaded;
    return 0;
	};

	conn->SetFileProgressCallback(progressCallback);
	conn->SetFileProgressCallbackData(NULL);

  RestClient::Response res = conn->get("/anything/{test_data}");
	EXPECT_EQ(200, res.code);
  EXPECT_GT(totalDownloaded, 0);
  EXPECT_EQ(totalDownloaded, totalToDownload);
}

TEST_F(ConnectionTestRemote, TestProxy)
{
  conn->SetProxy(RestClient::TestProxyUrl);
  RestClient::Response res = conn->get("/get");
  EXPECT_EQ(200, res.code);
}

TEST_F(ConnectionTestRemote, TestUnSetProxy)
{
  conn->SetProxy(RestClient::TestProxyUrl);
  conn->SetProxy("");
  RestClient::Response res = conn->get("/get");
  EXPECT_EQ(200, res.code);
}

TEST_F(ConnectionTestRemote, TestProxyAddressPrefixed)
{
  conn->SetProxy(RestClient::TestProxyUrl);
  RestClient::Response res = conn->get("/get");
  EXPECT_EQ(200, res.code);
}

TEST_F(ConnectionTest, TestInvalidProxy)
{
  conn->SetProxy("127.0.0.1:666");
  RestClient::Response res = conn->get("/get");
  EXPECT_EQ("Couldn't connect to server", res.body);
  EXPECT_EQ(CURLE_COULDNT_CONNECT, res.code);
}

TEST_F(ConnectionTest, TestTerminate)
{
  conn->Terminate();
  EXPECT_THROW({
      try
      {
          RestClient::Response res = conn->get("/get");
      }
      catch(const std::runtime_error& e)
      {
          EXPECT_STREQ("Connection terminated", e.what());
          throw;
      }
  }, std::runtime_error);
}

TEST_F(ConnectionTest, TestSetWriteFunction)
{
  static std::string lineReceived;
  static size_t lines = 0;
  static size_t ret = 0;

	auto writeCallback = [](void *data, size_t size, size_t nmemb, void *userdata) -> size_t
  {
    size_t bytes = size * nmemb;
    try
    {
        // Add to the buffer
        auto res = reinterpret_cast<RestClient::Response *>(userdata);
        res->body.append(static_cast<char*>(data), bytes);
        // If the last character is not a new line, wait for the rest.
        if ('\n' != *(res->body.end() - 1))
        {
            return bytes;
        }
        // Process data one line at a time.
        std::stringstream stream(res->body);
        std::string line;
        while (std::getline(stream, line))
        {
          // Do something with the line here...
          lineReceived += line;
          lines++;
        }
        // Done processing the line
        res->body.clear();
    }
    catch(std::exception e)
    {
        // Log caught exception here
        return 0;
    }
    ret = bytes;
    return bytes;
  };

  conn->SetWriteFunction(writeCallback);

  RestClient::Response res = {};
  auto ret_res = conn->get("/get", &res);

  EXPECT_EQ(ret_res, &res);
  EXPECT_EQ(200, res.code);
  EXPECT_EQ(ret, lineReceived.size() + lines);
}
