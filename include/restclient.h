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
    static response get(std::string& url);
    // HTTP POST
    static response post(std::string& url, std::string& ctype,
                         std::string& data);
    // HTTP PUT
    static response put(std::string& url, std::string& ctype,
                        std::string& data);
    // HTTP DELETE
    static response del(std::string& url);

  private:
    static size_t callback(void *ptr, size_t size, size_t nmemb,
                           void *userdata);


};
#endif // INCLUDE_RESTCLIENT_H_

