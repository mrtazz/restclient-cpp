#include <gtest/gtest.h>
#include <iostream>

#include "tests.h"

// Define shared constants
namespace RestClient {
std::string TestNonExistantUrl;
std::string TestServer;
std::string TestUrl;
std::string TestProxyUrl;
};  // namespace RestClient

int main(int argc, char** argv) {
  // Initialize shared constants
  // Ports below should match what is set in Makefile.in
  RestClient::TestNonExistantUrl = "http://nonexistent";
  RestClient::TestServer = "127.0.0.1:8998";
  RestClient::TestUrl = "http://" + RestClient::TestServer;
  RestClient::TestProxyUrl = "http://127.0.0.1:3128";

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
