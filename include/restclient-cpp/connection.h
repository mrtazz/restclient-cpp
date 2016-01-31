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

#include "restclient-cpp/restclient.h"
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
      *  @struct RequestInfo
      *  @brief holds some diagnostics information
      *  about a request
      */
    typedef struct {
        // total time of the last request in seconds Total time of previous
        // transfer. See CURLINFO_TOTAL_TIME
        double totalTime;
        // time spent in DNS lookup in seconds Time from start until name
        // resolving completed. See CURLINFO_NAMELOOKUP_TIME
        double nameLookupTime;
        // time it took until Time from start until remote host or proxy
        // completed. See CURLINFO_CONNECT_TIME
        double connectTime;
        // Time from start until SSL/SSH handshake completed. See
        // CURLINFO_APPCONNECT_TIME
        double appConnectTime;
        // Time from start until just before the transfer begins. See
        // CURLINFO_PRETRANSFER_TIME
        double preTransferTime;
        // Time from start until just when the first byte is received. See
        // CURLINFO_STARTTRANSFER_TIME
        double startTransferTime;
        // Time taken for all redirect steps before the final transfer. See
        // CURLINFO_REDIRECT_TIME
        double redirectTime;
        // number of redirects followed. See CURLINFO_REDIRECT_COUNT
        int redirectCount;
      } RequestInfo;
    /**
      *  @struct Info
      *  @brief holds some diagnostics information
      *  about the connection object it came from
      */
    typedef struct {
      std::string baseUrl;
      RestClient::HeaderFields headers;
      int timeout;
      bool followRedirects;
      struct {
        std::string username;
        std::string password;
      } basicAuth;
      std::string customUserAgent;
      RequestInfo lastRequest;
    } Info;


    explicit Connection(const std::string baseUrl);
    ~Connection();

    // Instance configuration methods
    // configure basic auth
    void SetBasicAuth(const std::string& username,
                      const std::string& password);

    // set connection timeout to seconds
    void SetTimeout(int seconds);

    // set whether to follow redirects
    // TODO(mrtazz): implement this
    void FollowRedirects(bool follow);

    // set custom user agent
    // (this will result in the UA "foo/cool restclient-cpp/VERSION")
    void SetUserAgent(const std::string& userAgent);

    std::string GetUserAgent();

    RestClient::Connection::Info GetInfo();

    // set headers
    void SetHeaders(RestClient::HeaderFields headers);

    // get headers
    RestClient::HeaderFields GetHeaders();

    // append additional headers
    void AppendHeader(const std::string& key,
                      const std::string& value);


    // Basic HTTP verb methods
    RestClient::Response get(const std::string& uri);
    RestClient::Response post(const std::string& uri,
                              const std::string& data);
    RestClient::Response put(const std::string& uri,
                             const std::string& data);
    RestClient::Response del(const std::string& uri);

 private:
    CURL* curlHandle;
    std::string baseUrl;
    RestClient::HeaderFields headerFields;
    int timeout;
    bool followRedirects;
    struct {
      std::string username;
      std::string password;
    } basicAuth;
    std::string customUserAgent;
    RequestInfo lastRequest;
    RestClient::Response performCurlRequest(const std::string& uri);
};
};  // namespace RestClient

#endif  // INCLUDE_RESTCLIENT_CPP_CONNECTION_H_
