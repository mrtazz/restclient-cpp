/**
 * @file connection.h
 * @brief header definitions for restclient-cpp connection class
 * @author Daniel Schauenberg <d@unwiredcouch.com>
 * @version
 * @date 2010-10-11
 */

#ifndef INCLUDE_RESTCLIENT_CPP_CONNECTION_H_
#define INCLUDE_RESTCLIENT_CPP_CONNECTION_H_

#include <curl/curl.h>
#include <string>
#include <map>
#include <cstdlib>

#include "restclient-cpp/version.h"

/**
 * @brief namespace for all RestClient definitions
 */
namespace RestClient {

/**
  * @brief Connection object for advanced usage
  */
class Connection {
 public:
    /**
      * @brief enum to hold identifiers for HTTP verbs
      */
    enum HttpVerb {
      GET,
      POST,
      PUT,
      DELETE
    };

    /**
      *  @struct Info
      *  @brief holds some diagnostics information
      *  about the connection object it came from
      */
    typedef struct {
      std::string base_url;
      HeaderFields headers;
      int timeout;
      struct {
        std::string username;
        std::string password;
      } basicAuth;
      std::string customUserAgent;
    } Info;


    explicit Connection(const std::string& baseUrl);
    ~Connection();

    // Instance configuration methods
    // configure basic auth
    void SetBasicAuth(const std::string& username,
                      const std::string& password);

    // set connection timeout to 5s
    void SetTimeout(int seconds);

    // set custom user agent
    // (this will result in the UA "foo/cool restclient-cpp/VERSION")
    void SetUserAgent(const std::string& userAgent);

    // set headers
    void SetHeaders(headermap headers);

    // append additional headers
    void AppendHeader(const std::string& key,
                      const std::string& value);


    // Basic HTTP verb methods
    response get(const std::string& uri);
    response post(const std::string& uri,
                  const std::string& contentType,
                  const std::string& data);
    response put(const std::string& uri,
                  const std::string& contentType,
                  const std::string& data);
    response del(const std::string& uri);

 private:
    CURL* curlHandle = NULL;
    std::string baseUrl;
    headermap headers;
    int timeout;
    struct {
      std::string username;
      std::string password;
    } basicAuth;
    std::string customUserAgent;
};
};  // namespace RestClient

#endif  // INCLUDE_RESTCLIENT_CPP_CONNECTION_H_
