/**
 * @file restclient.cpp
 * @brief implementation of the restclient class
 * @author Daniel Schauenberg <d@unwiredcouch.com>
 */

/*========================
         INCLUDES
  ========================*/
#include "include/restclient.h"

#include <string>
#include <iostream>

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
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RestClient::write_callback);
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
  /** build content-type header string */
  std::string ctype_header = "Content-Type: " + ctype;

  /** initialize upload object */
  RestClient::upload_object up_obj;
  up_obj.data = data;
  up_obj.length = data.size();
  up_obj.transmitted = 0;

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
    /** Now specify we want to POST data */
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    /** set read callback function */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, RestClient::read_callback);
    /** set data object to pass to callback function */
    curl_easy_setopt(curl, CURLOPT_READDATA, &up_obj);
    /** set callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RestClient::write_callback);
    /** set data object to pass to callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
    /** set content-type header */
    curl_slist* header = NULL;
    header = curl_slist_append(header, ctype_header.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
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
 * @brief write callback function for libcurl
 *
 * @param data returned data of size (size*nmemb)
 * @param size size parameter
 * @param nmemb memblock parameter
 * @param userdata pointer to user data to save/work with return data
 *
 * @return (size * nmemb)
 */
size_t RestClient::write_callback(void *data, size_t size, size_t nmemb,
                            void *userdata)
{
  RestClient::response* r;
  r = reinterpret_cast<RestClient::response*>(userdata);
  r->body.append(reinterpret_cast<char*>(data), size*nmemb);

  return (size * nmemb);
}

/**
 * @brief read callback function for libcurl
 *
 * @param pointer of max size (size*nmemb) to write data to
 * @param size size parameter
 * @param nmemb memblock parameter
 * @param userdata pointer to user data to read data from
 *
 * @return (size * nmemb)
 */
size_t RestClient::read_callback(void *data, size_t size, size_t nmemb,
                            void *userdata)
{
  int max_transmit_length;
  RestClient::upload_object* u;
  u = reinterpret_cast<RestClient::upload_object*>(userdata);
  // check if we can transmit all at once
  if ((size*nmemb) >= (u->data.size() - u->transmitted))
  {
    max_transmit_length = u->data.size() - u->transmitted;
  }
  // if not only transmit the size of the buffer
  else
  {
    max_transmit_length = (size*nmemb) + u->transmitted;
  }
  for (int i=u->transmitted; i < max_transmit_length; ++i)
  {
    reinterpret_cast<char*>(data)[i] = u->data.c_str()[i];
    ++u->transmitted;
  }
  // return 0 if we have submitted all bytes
  if ((u->data.size() - u->transmitted) == 0)
  {
    return 0;
  }
  else  // return the transmitted bytes otherwise
  {
    return u->transmitted;
  }
}
