/**
 * @file restclient.cpp
 * @brief implementation of the restclient class
 * @author Daniel Schauenberg <d@unwiredcouch.com>
 */

/*========================
         INCLUDES
  ========================*/
#include <string>
#include <iostream>
#include "include/restclient.h"

/** initialize user agent string */
const char* RestClient::user_agent = "restclient-cpp/" VERSION;
/**
 * @brief HTTP GET method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::response RestClient::get(const std::string& url)
{
  /** create return struct */
  RestClient::response ret;

  // use libcurl
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if (curl)
  {
    /** set user agent */
    curl_easy_setopt(curl, CURLOPT_USERAGENT, RestClient::user_agent);
    /** set query URL */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    /** set callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RestClient::callback);
    /** set data object to pass to callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
    /** perform the actual query */
    res = curl_easy_perform(curl);
    if (res != 0)
    {
      std::cerr << "Failed to query " << url << ":"
          << std::endl << curl_easy_strerror(res) << std::endl << std::flush;
      exit(1);
    }
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    ret.code = static_cast<int>(http_code);

    curl_easy_cleanup(curl);
  }

  return ret;
}
/**
 * @brief HTTP POST method
 *
 * @param url to query
 * @param ctype content type as string
 * @param data HTTP POST body
 *
 * @return response struct
 */
RestClient::response RestClient::post(const std::string& url,
                                      const std::string& ctype,
                                      const std::string& data)
{
  /** create return struct */
  RestClient::response ret;
  return ret;
}
/**
 * @brief HTTP PUT method
 *
 * @param url to query
 * @param ctype content type as string
 * @param data HTTP PUT body
 *
 * @return response struct
 */
RestClient::response RestClient::put(const std::string& url,
                                     const std::string& ctype,
                                     const std::string& data)
{
  /** create return struct */
  RestClient::response ret;
  return ret;
}
/**
 * @brief HTTP DELETE method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::response RestClient::del(const std::string& url)
{
  /** create return struct */
  RestClient::response ret;
  return ret;
}

/**
 * @brief callback function for libcurl
 *
 * @param data returned data of size (size*nmemb)
 * @param size size parameter
 * @param nmemb memblock parameter
 * @param userdata pointer to user data to save/work with return data
 *
 * @return (size * nmemb)
 */
size_t RestClient::callback(void *data, size_t size, size_t nmemb,
                            void *userdata)
{
  reinterpret_cast<RestClient::response*>(userdata)->body.append((char*)data,
                                                            size*nmemb);
  return (size * nmemb);
}
