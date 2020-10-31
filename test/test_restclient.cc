#include "restclient-cpp/restclient.h"
#include <gtest/gtest.h>
#include <json/json.h>
#include <cstdio>
#include <fstream>
#include <string>
#include <sstream>

#include "tests.h"

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
  RestClient::Response res = RestClient::del(RestClient::TestUrl+"/delete");
  EXPECT_EQ(200, res.code);
}

TEST_F(RestClientTest, TestRestClientDELETEBody)
{
  RestClient::Response res = RestClient::del(RestClient::TestUrl+"/delete");
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ(RestClient::TestUrl+"/delete", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" RESTCLIENT_VERSION, root["headers"].get("User-Agent", "no url set").asString());
}

// check for failure
TEST_F(RestClientTest, TestRestClientDELETEFailureCode)
{
  std::string u = RestClient::TestNonExistantUrl;
  RestClient::Response res = RestClient::del(u);
  // 6 = CURLE_COULDNT_RESOLVE_HOST 
  EXPECT_EQ(6, res.code);
}

TEST_F(RestClientTest, TestRestClientDELETEHeaders)
{
  RestClient::Response res = RestClient::del(RestClient::TestUrl+"/delete");
  EXPECT_EQ("keep-alive", res.headers["Connection"]);
}

// GET Tests
TEST_F(RestClientTest, TestRestClientGETCode)
{
  RestClient::Response res = RestClient::get(RestClient::TestUrl+"/get");
  EXPECT_EQ(200, res.code);
}

TEST_F(RestClientTest, TestRestClientGETHTTP2Code)
{
  RestClient::Response res = RestClient::get("https://http2.golang.org/reqinfo");
  EXPECT_EQ(200, res.code);
}

TEST_F(RestClientTest, TestRestClientGETBodyCode)
{
  RestClient::Response res = RestClient::get(RestClient::TestUrl+"/get");
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ(RestClient::TestUrl+"/get", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" RESTCLIENT_VERSION, root["headers"].get("User-Agent", "no url set").asString());
}

// check for failure
TEST_F(RestClientTest, TestRestClientGETFailureCode)
{
  std::string u = RestClient::TestNonExistantUrl;
  RestClient::Response res = RestClient::get(u);
  EXPECT_EQ("Couldn't resolve host name", res.body);
  // 6 = CURLE_COULDNT_RESOLVE_HOST 
  EXPECT_EQ(6, res.code);
}

TEST_F(RestClientTest, TestRestClientGETHeaders)
{
  RestClient::Response res = RestClient::get(RestClient::TestUrl+"/get");
  EXPECT_EQ("keep-alive", res.headers["Connection"]);
}

// POST Tests
// check return code
TEST_F(RestClientTest, TestRestClientPOSTCode)
{
  RestClient::Response res = RestClient::post(RestClient::TestUrl+"/post", "text/text", "data");
  EXPECT_EQ(200, res.code);
}

TEST_F(RestClientTest, TestRestClientPOSTBody)
{
  RestClient::Response res = RestClient::post(RestClient::TestUrl+"/post", "text/text", "data");
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ(RestClient::TestUrl+"/post", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" RESTCLIENT_VERSION, root["headers"].get("User-Agent", "no url set").asString());
}
TEST_F(RestClientTest, TestRestClientPostForm)
{
  // generating a file name with a timestamp
  std::ostringstream fileName;
  time_t rawtime;
  tm * timeinfo;
  time(&rawtime);
  timeinfo = localtime( &rawtime );
  
  fileName << "TestPostForm_" << (timeinfo->tm_year)+1900 << "_" << timeinfo->tm_mon+1
  << "_" << timeinfo->tm_mday << "-" << timeinfo->tm_hour
  << "_"<< timeinfo->tm_min << "_" << timeinfo->tm_sec << ".txt";

  // creating a dummy file to upload via a post form request
  std::ofstream ofDummyFile(fileName.str().c_str());
  ASSERT_TRUE(static_cast<bool>(ofDummyFile));
  ofDummyFile << "Dummy file for the unit test 'TestRestClientPostForm' of the restclient-cpp Project.";
  ASSERT_TRUE(static_cast<bool>(ofDummyFile));
  ofDummyFile.close();

  // uploading the dummy file
  RestClient::FormData UploadInfo;
  UploadInfo.addFormFile("submitted", fileName.str());
  UploadInfo.addFormContent("filename", fileName.str());
  RestClient::Response res = RestClient::post("http://ptsv2.com/t/restclientcpptests/post", UploadInfo);
  EXPECT_EQ(200, res.code);

  // remove dummy file
  remove(fileName.str().c_str());
}

// check for failure
TEST_F(RestClientTest, TestRestClientPOSTFailureCode)
{
  std::string u = RestClient::TestNonExistantUrl;
  RestClient::Response res = RestClient::post(u, "text/text", "data");
  // 6 = CURLE_COULDNT_RESOLVE_HOST 
  EXPECT_EQ(6, res.code);
}

TEST_F(RestClientTest, TestRestClientPOSTHeaders)
{
  RestClient::Response res = RestClient::post(RestClient::TestUrl+"/post", "text/text", "data");
  EXPECT_EQ("keep-alive", res.headers["Connection"]);
}

// PUT Tests
// check return code
TEST_F(RestClientTest, TestRestClientPUTCode)
{
  RestClient::Response res = RestClient::put(RestClient::TestUrl+"/put", "text/text", "data");
  EXPECT_EQ(200, res.code);
}

TEST_F(RestClientTest, TestRestClientPUTBody)
{
  RestClient::Response res = RestClient::put(RestClient::TestUrl+"/put", "text/text", "data");
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ(RestClient::TestUrl+"/put", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" RESTCLIENT_VERSION, root["headers"].get("User-Agent", "no url set").asString());
}

// check for failure
TEST_F(RestClientTest, TestRestClientPUTFailureCode)
{
  std::string u = RestClient::TestNonExistantUrl;
  RestClient::Response res = RestClient::put(u, "text/text", "data");
  // 6 = CURLE_COULDNT_RESOLVE_HOST 
  EXPECT_EQ(6, res.code);
}

TEST_F(RestClientTest, TestRestClientPUTHeaders)
{
  RestClient::Response res = RestClient::put(RestClient::TestUrl+"/put", "text/text", "data");
  EXPECT_EQ("keep-alive", res.headers["Connection"]);
}

// PATCH Tests
// check return code
TEST_F(RestClientTest, TestRestClientPATCHCode)
{
  RestClient::Response res = RestClient::patch(RestClient::TestUrl+"/patch", "text/text", "data");
  EXPECT_EQ(200, res.code);
}

TEST_F(RestClientTest, TestRestClientPATCHBody)
{
  RestClient::Response res = RestClient::patch(RestClient::TestUrl+"/patch", "text/text", "data");
  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ(RestClient::TestUrl+"/patch", root.get("url", "no url set").asString());
  EXPECT_EQ("restclient-cpp/" RESTCLIENT_VERSION, root["headers"].get("User-Agent", "no url set").asString());
}

// check for failure
TEST_F(RestClientTest, TestRestClientPATCHFailureCode)
{
  std::string u = RestClient::TestNonExistantUrl;
  RestClient::Response res = RestClient::patch(u, "text/text", "data");
  // 6 = CURLE_COULDNT_RESOLVE_HOST 
  EXPECT_EQ(6, res.code);
}

TEST_F(RestClientTest, TestRestClientPATCHHeaders)
{
  RestClient::Response res = RestClient::patch(RestClient::TestUrl+"/put", "text/text", "data");
  EXPECT_EQ("keep-alive", res.headers["Connection"]);
}

// OPTIONS Tests
// check return code
// Disabled as httpbin does not support options requests for now
TEST_F(RestClientTest, TestRestClientOPTIONSCode)
{
  RestClient::Response res = RestClient::options("https://api.reqbin.com/api/v1/requests");
  EXPECT_EQ(200, res.code);
}

// check for failure
TEST_F(RestClientTest, TestRestClientOPTIONSFailureCode)
{
  std::string u = RestClient::TestNonExistantUrl;
  RestClient::Response res = RestClient::options(u);
  // 6 = CURLE_COULDNT_RESOLVE_HOST 
  EXPECT_EQ(6, res.code);
}

TEST_F(RestClientTest, TestRestClientOPTIONSHeaders)
{
  RestClient::Response res = RestClient::options(
      RestClient::TestUrl+"/options");
  EXPECT_EQ("keep-alive", res.headers["Connection"]);
}

TEST_F(RestClientTest, TestRestClientAuth)
{
  RestClient::Response res = RestClient::get("http://foo:bar@" + RestClient::TestServer + "/basic-auth/foo/bar");
  EXPECT_EQ(200, res.code);

  Json::Value root;
  std::istringstream str(res.body);
  str >> root;

  EXPECT_EQ("foo", root.get("user", "no user").asString());
  EXPECT_EQ(true, root.get("authenticated", false).asBool());

  res = RestClient::get("http://" + RestClient::TestServer + "/basic-auth/foo/bar");
  EXPECT_EQ(401, res.code);
}

TEST_F(RestClientTest, TestRestClientHeadCode)
{
  RestClient::Response res = RestClient::head(RestClient::TestUrl+"/get");
  EXPECT_EQ(200, res.code);
  EXPECT_EQ("", res.body);
}
