#include "restclient-cpp/restclient.h"
#include "restclient-cpp/connection.h"
#include <gtest/gtest.h>
#include <json/json.h>
#include <string>
#include <fstream>
#include <cstdio>

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
  EXPECT_EQ(-CURLE_OPERATION_TIMEDOUT, res.code);
}

TEST_F(ConnectionTest, TestFailForInvalidCA)
{
  // set a non-existing file for the CA file and it should fail to verify the peer
  conn->SetCAInfoFilePath("non-existent file");
  RestClient::Response res = conn->get("/get");

  EXPECT_EQ("Failed to query; curl error code: 77", res.body);
  EXPECT_EQ(-CURLE_SSL_CACERT_BADFILE, res.code);
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

TEST_F(ConnectionTest, TestPostToFile)
{
  std::string testFile = "outfile";
  remove(testFile.c_str()); // make sure we have a clean setup
  conn->OutputToFile(testFile);
  RestClient::Response res = conn->post("/post", "data");
  EXPECT_EQ(200, res.code);
  // the response body should be empty
  EXPECT_TRUE(res.body.empty());
  // the response should be in the file
  Json::Value root;
  std::fstream str(testFile, std::ios::in);
  str >> root;
  EXPECT_EQ("https://httpbin.org/post", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" RESTCLIENT_VERSION, root["headers"].get("User-Agent", "no url set").asString());
  // clean up
  remove(testFile.c_str());
}

TEST_F(ConnectionTest, TestPostToFileError)
{
  std::string testFile = "/no/such/path/exists!/";
  conn->OutputToFile(testFile);
  RestClient::Response res = conn->post("/post", "data");
  EXPECT_EQ(-CURLE_WRITE_ERROR, res.code);
}


TEST_F(ConnectionTest, TestGetToFile)
{
  std::string testFile = "outfile";
  remove(testFile.c_str()); // make sure we have a clean setup
  conn->OutputToFile(testFile);
  RestClient::Response res = conn->get("/get");
  EXPECT_EQ(200, res.code);
  // the response body should be empty
  EXPECT_TRUE(res.body.empty());
  // the response should be in the file
  Json::Value root;
  std::fstream str(testFile, std::ios::in);
  str >> root;
  EXPECT_EQ("https://httpbin.org/get", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" RESTCLIENT_VERSION, root["headers"].get("User-Agent", "no url set").asString());
  // clean up
  remove(testFile.c_str());
}

TEST_F(ConnectionTest, TestGetToFileProgress)
{
  std::string testFile = "outfile";
  remove(testFile.c_str()); // make sure we have a clean setup
  conn->OutputToFile(testFile);
  // add a progress callback
  long long int lastDownloadedBytes = 0;
  conn->SetProgressCallback([&lastDownloadedBytes](long long totalDownloadBytes, long long downloadedBytes, long long totalUploadBytes, long long uploadedBytes) mutable {
    // the progress should be increasing
    EXPECT_TRUE(downloadedBytes >= lastDownloadedBytes);
    lastDownloadedBytes = downloadedBytes;
  });
  RestClient::Response res = conn->get("/image/png");
  EXPECT_EQ(200, res.code);
  // check whether the progress reached the total file size
  EXPECT_EQ(8090, lastDownloadedBytes);
  // clean up
  remove(testFile.c_str());
}

TEST_F(ConnectionTest, TestPutToFile)
{
  std::string testFile = "outfile";
  remove(testFile.c_str()); // make sure we have a clean setup
  conn->OutputToFile(testFile);
  RestClient::Response res = conn->put("/put", "data");
  EXPECT_EQ(200, res.code);
  // the response body should be empty
  EXPECT_TRUE(res.body.empty());
  // the response should be in the file
  Json::Value root;
  std::fstream str(testFile, std::ios::in);
  str >> root;
  EXPECT_EQ("https://httpbin.org/put", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" RESTCLIENT_VERSION, root["headers"].get("User-Agent", "no url set").asString());
  // clean up
  remove(testFile.c_str());
}

TEST_F(ConnectionTest, TestPostFromFile)
{
  std::string testFile = "test/testdata.txt";
  conn->AppendHeader("Content-Type", "text/text");
  conn->InputFromFile(testFile);
  RestClient::Response res = conn->post("/post", "");
  EXPECT_EQ(200, res.code);
  // get the test data from file
  std::ifstream fstr(testFile);
  std::stringstream buffer;
  buffer << fstr.rdbuf();
  std::string testData = buffer.str();
  // check response
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;
  EXPECT_EQ(testData, root.get("data", "no data set").asString());
  EXPECT_EQ("https://httpbin.org/post", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" RESTCLIENT_VERSION, root["headers"].get("User-Agent", "no user agent set").asString());
}

TEST_F(ConnectionTest, TestPostFromFileError)
{
  std::string testFile = "no-such-file";
  conn->AppendHeader("Content-Type", "text/text");
  conn->InputFromFile(testFile);
  RestClient::Response res = conn->post("/post", "");
  EXPECT_EQ(-CURLE_READ_ERROR, res.code);
}

TEST_F(ConnectionTest, TestPostFromFileProgress)
{
  std::string testFile = "test/testdata.txt";
  conn->InputFromFile(testFile);
  // add a progress callback
  long long int lastUploadedBytes = 0;
  conn->SetProgressCallback([&lastUploadedBytes](long long totalDownloadBytes, long long downloadedBytes, long long totalUploadBytes, long long uploadedBytes) mutable {
    // the progress should be increasing
    EXPECT_TRUE(uploadedBytes >= lastUploadedBytes);
    lastUploadedBytes = uploadedBytes;
  });
  RestClient::Response res = conn->post("/post", "");
  EXPECT_EQ(200, res.code);
  // get the test data size
  std::ifstream fstr(testFile, std::ifstream::ate | std::ifstream::binary);
  long dataSize = fstr.tellg();
  // check whether the progress reached the upload size
  EXPECT_EQ(dataSize, lastUploadedBytes);
}

TEST_F(ConnectionTest, TestPutFromFile)
{
  std::string testFile = "test/testdata.txt";
  conn->InputFromFile(testFile);
  RestClient::Response res = conn->put("/put", "");
  EXPECT_EQ(200, res.code);
  // get the test data from file
  std::ifstream fstr(testFile);
  std::stringstream buffer;
  buffer << fstr.rdbuf();
  std::string testData = buffer.str();
  // check response
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;
  EXPECT_EQ(testData, root.get("data", "no data set").asString());
  EXPECT_EQ("https://httpbin.org/put", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" RESTCLIENT_VERSION, root["headers"].get("User-Agent", "no user agent set").asString());
}

TEST_F(ConnectionTest, TestPostFromFileToFile)
{
  std::string testInFile = "test/testdata.txt";
  conn->AppendHeader("Content-Type", "text/text");
  conn->InputFromFile(testInFile);
  std::string testOutFile = "outfile";
  remove(testOutFile.c_str()); // make sure we have a clean setup
  conn->OutputToFile(testOutFile);
  RestClient::Response res = conn->post("/post", "");
  EXPECT_EQ(200, res.code);
  // get the test data from file
  std::ifstream fstr(testInFile);
  std::stringstream buffer;
  buffer << fstr.rdbuf();
  std::string testData = buffer.str();
  // check response
  EXPECT_TRUE(res.body.empty());
  // the response should be in the file
  Json::Value root;
  std::fstream str(testOutFile, std::ios::in);
  str >> root;
  EXPECT_EQ(testData, root.get("data", "no data set").asString());
  EXPECT_EQ("https://httpbin.org/post", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" RESTCLIENT_VERSION, root["headers"].get("User-Agent", "no user agent set").asString());
  // clean up
  remove(testOutFile.c_str());
}

