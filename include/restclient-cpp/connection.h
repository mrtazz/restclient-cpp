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
      *  @struct Info
      *  @brief holds some diagnostics information
      *  about the connection object it came from
      */
    typedef struct {
      std::string base_url;
      RestClient::HeaderFields headers;
      int timeout;
      struct {
        std::string username;
        std::string password;
      } basicAuth;
      std::string customUserAgent;
      struct {
        // total time of the last request in seconds Total time of previous
        // transfer. See CURLINFO_TOTAL_TIME
        int totalTime;
        // time spent in DNS lookup in seconds Time from start until name
        // resolving completed. See CURLINFO_NAMELOOKUP_TIME
        int nameLookupTime;
        // time it took until Time from start until remote host or proxy
        // completed. See CURLINFO_CONNECT_TIME
        int connectTime;
        // Time from start until SSL/SSH handshake completed. See
        // CURLINFO_APPCONNECT_TIME
        int appConnectTime;
        // Time from start until just before the transfer begins. See
        // CURLINFO_PRETRANSFER_TIME
        int preTransferTime;
        // Time from start until just when the first byte is received. See
        // CURLINFO_STARTTRANSFER_TIME
        int startTransferTime;
        // Time taken for all redirect steps before the final transfer. See
        // CURLINFO_REDIRECT_TIME
        int redirectTime;
        // number of redirects followed. See CURLINFO_REDIRECT_COUNT
        int redirectCount;
      } lastRequest;
    } Info;


    explicit Connection(const std::string baseUrl);
    ~Connection();

    // Instance configuration methods
    // configure basic auth
    void SetBasicAuth(const std::string& username,
                      const std::string& password);

    // set connection timeout to seconds
    void SetTimeout(int seconds);

    // set custom user agent
    // (this will result in the UA "foo/cool restclient-cpp/VERSION")
    void SetUserAgent(const std::string& userAgent);

    std::string GetUserAgent();

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
    struct {
      std::string username;
      std::string password;
    } basicAuth;
    std::string customUserAgent;
    Info infoStruct;
    RestClient::Response performCurlRequest(const std::string& uri);
};
};  // namespace RestClient

#endif  // INCLUDE_RESTCLIENT_CPP_CONNECTION_H_
