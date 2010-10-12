/**
 * @file restclient.h
 * @brief libcurl wrapper for REST calls
 * @author Daniel Schauenberg <d@unwiredcouch.com>
 * @version
 * @date 2010-10-11
 */

#ifndef INCLUDE_RESTCLIENT_H_
#define INCLUDE_RESTCLIENT_H_

#include <string>
#include <curl/curl.h>

class RestClient
{
  public:
    /**
     * public data definitions
     */
    /** response struct for queries */
    typedef struct
    {
      int code;
      std::string body;
    } response;

    /** public methods */
    // HTTP GET
    static response get(const std::string& url);
    // HTTP POST
    static response post(const std::string& url, const std::string& ctype,
                         const std::string& data);
    // HTTP PUT
    static response put(const std::string& url, const std::string& ctype,
                        const std::string& data);
    // HTTP DELETE
    static response del(const std::string& url);

  private:
    static size_t callback(void *ptr, size_t size, size_t nmemb,
                           void *userdata);


};
#endif // INCLUDE_RESTCLIENT_H_

