/**
 * @file restclient.cpp
 * @brief implementation of the restclient class
 * @author Daniel Schauenberg <d@unwiredcouch.com>
 */

/*========================
         INCLUDES
  ========================*/
#include "restclient-cpp/restclient.h"
#include "restclient-cpp/meta.h"

#include <curl/curl.h>
#include <cstring>
#include <string>
#include <iostream>
#include <map>

/** initialize user agent string */
const char* RestClient::user_agent = "restclient-cpp/" VERSION;
/** initialize authentication variable */
std::string RestClient::user_pass =  std::string();
/** Authentication Methods implementation */
void RestClient::clearAuth(){
  RestClient::user_pass.clear();
}
void RestClient::setAuth(const std::string& user,const std::string& password){
  RestClient::user_pass.clear();
  RestClient::user_pass += user+":"+password;
}
/**
 * @brief HTTP GET method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::response RestClient::get(const std::string& url, const size_t timeout)
{
  headermap emptyMap;
  return RestClient::get(url, emptyMap, timeout);
}

/**
 * @brief HTTP GET method
 *
 * @param url to query
 * @param headers HTTP headers
 *
 * @return response struct
 */
RestClient::response RestClient::get(const std::string& url, const headermap& headers, const size_t timeout)
{
  /** create return struct */
  RestClient::response ret = {};

  // use libcurl
  CURL *curl = NULL;
  CURLcode res = CURLE_OK;
  std::string header;

  curl = curl_easy_init();
  if (curl)
  {
    /** set basic authentication if present*/
    if(RestClient::user_pass.length()>0){
      curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
      curl_easy_setopt(curl, CURLOPT_USERPWD, RestClient::user_pass.c_str());
    }
    /** set user agent */
    curl_easy_setopt(curl, CURLOPT_USERAGENT, RestClient::user_agent);
    /** set query URL */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    /** set callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RestClient::write_callback);
    /** set data object to pass to callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
    /** set the header callback function */
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, RestClient::header_callback);
    /** callback object for headers */
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &ret);
    /** set http headers */
    curl_slist* hlist = NULL;
    for (headermap::const_iterator it = headers.begin(); it != headers.end(); ++it) {
      header = it->first;
      header += ": ";
      header += it->second;
      hlist = curl_slist_append(hlist, header.c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hlist);
    /** perform the actual query */

    //set timeout
    if (timeout) {
      curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
      curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1); // dont want to get a sig alarm on timeout
    }

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
	  if (res == CURLE_OPERATION_TIMEDOUT) {
		ret.code = res;
		ret.body = "Operation Timeout.";
		return ret;
	  }

      ret.body = "Failed to query.";
      ret.code = -1;
      return ret;
    }
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    ret.code = static_cast<int>(http_code);

    curl_slist_free_all(hlist);
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
                                      const std::string& data,
                                      const size_t timeout)
{
    headermap emptyMap;
    return RestClient::post(url, ctype, data, emptyMap, timeout);
}

/**
 * @brief HTTP POST method
 *
 * @param url to query
 * @param ctype content type as string
 * @param data HTTP POST body
 * @param headers HTTP headers
 *
 * @return response struct
 */
RestClient::response RestClient::post(const std::string& url,
                                      const std::string& ctype,
                                      const std::string& data,
                                      const headermap& headers,
                                      const size_t timeout)
{
  /** create return struct */
  RestClient::response ret = {};
  /** build content-type header string */
  std::string ctype_header = "Content-Type: " + ctype;
  std::string header;

  // use libcurl
  CURL *curl = NULL;
  CURLcode res = CURLE_OK;

  curl = curl_easy_init();
  if (curl)
  {
    /** set basic authentication if present*/
    if(RestClient::user_pass.length()>0){
      curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
      curl_easy_setopt(curl, CURLOPT_USERPWD, RestClient::user_pass.c_str());
    }
    /** set user agent */
    curl_easy_setopt(curl, CURLOPT_USERAGENT, RestClient::user_agent);
    /** set query URL */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    /** Now specify we want to POST data */
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    /** set post fields */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
    /** set callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RestClient::write_callback);
    /** set data object to pass to callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
    /** set the header callback function */
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, RestClient::header_callback);
    /** callback object for headers */
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &ret);
    /** set content-type header */
    curl_slist* hlist = NULL;
    hlist = curl_slist_append(hlist, ctype_header.c_str());
    for (headermap::const_iterator it = headers.begin(); it != headers.end(); ++it) {
      header = it->first;
      header += ": ";
      header += it->second;
      hlist = curl_slist_append(hlist, header.c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hlist);

    //set timeout
    if (timeout) {
      curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
      curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1); // dont want to get a sig alarm on timeout
    }

    /** perform the actual query */
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
	  if (res == CURLE_OPERATION_TIMEDOUT) {
		ret.code = res;
		ret.body = "Operation Timeout.";
		return ret;
	  }

      ret.body = "Failed to query.";
      ret.code = -1;
      return ret;
    }
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    ret.code = static_cast<int>(http_code);

    curl_slist_free_all(hlist);
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
                                     const std::string& data,
                                     const size_t timeout)
{
    headermap emptyMap;
    return RestClient::put(url, ctype, data, emptyMap, timeout);
}

/**
 * @brief HTTP PUT method
 *
 * @param url to query
 * @param ctype content type as string
 * @param data HTTP PUT body
 * @param headers HTTP headers
 *
 * @return response struct
 */
RestClient::response RestClient::put(const std::string& url,
                                     const std::string& ctype,
                                     const std::string& data,
                                     const headermap& headers,
                                     const size_t timeout)
{
  /** create return struct */
  RestClient::response ret = {};
  /** build content-type header string */
  std::string ctype_header = "Content-Type: " + ctype;
  std::string header;

  /** initialize upload object */
  RestClient::upload_object up_obj;
  up_obj.data = data.c_str();
  up_obj.length = data.size();

  // use libcurl
  CURL *curl = NULL;
  CURLcode res = CURLE_OK;

  curl = curl_easy_init();
  if (curl)
  {
    /** set basic authentication if present*/
    if(RestClient::user_pass.length()>0){
      curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
      curl_easy_setopt(curl, CURLOPT_USERPWD, RestClient::user_pass.c_str());
    }
    /** set user agent */
    curl_easy_setopt(curl, CURLOPT_USERAGENT, RestClient::user_agent);
    /** set query URL */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    /** Now specify we want to PUT data */
    curl_easy_setopt(curl, CURLOPT_PUT, 1L);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    /** set read callback function */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, RestClient::read_callback);
    /** set data object to pass to callback function */
    curl_easy_setopt(curl, CURLOPT_READDATA, &up_obj);
    /** set callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RestClient::write_callback);
    /** set data object to pass to callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
    /** set the header callback function */
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, RestClient::header_callback);
    /** callback object for headers */
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &ret);
    /** set data size */
    curl_easy_setopt(curl, CURLOPT_INFILESIZE,
                     static_cast<long>(up_obj.length));

    /** set content-type header */
    curl_slist* hlist = NULL;
    hlist = curl_slist_append(hlist, ctype_header.c_str());
    for (headermap::const_iterator it = headers.begin(); it != headers.end(); ++it) {
      header = it->first;
      header += ": ";
      header += it->second;
      hlist = curl_slist_append(hlist, header.c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hlist);

    //set timeout
    if (timeout) {
      curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
      curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1); // dont want to get a sig alarm on timeout
    }

    /** perform the actual query */
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
	  if (res == CURLE_OPERATION_TIMEDOUT) {
		ret.code = res;
		ret.body = "Operation Timeout.";
		return ret;
	  }

      ret.body = "Failed to query.";
      ret.code = -1;
      return ret;
    }
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    ret.code = static_cast<int>(http_code);

    curl_slist_free_all(hlist);
    curl_easy_cleanup(curl);
  }

  return ret;
}
/**
 * @brief HTTP DELETE method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::response RestClient::del(const std::string& url, const size_t timeout)
{
    headermap emptyMap;
    return RestClient::del(url, emptyMap, timeout);
}

/**
 * @brief HTTP DELETE method
 *
 * @param url to query
 * @param headers HTTP headers
 *
 * @return response struct
 */
RestClient::response RestClient::del(const std::string& url,
                                     const headermap& headers,
                                     const size_t timeout)
{
  /** create return struct */
  RestClient::response ret = {};

  /** we want HTTP DELETE */
  const char* http_delete = "DELETE";

  // use libcurl
  CURL *curl = NULL;
  CURLcode res = CURLE_OK;
  std::string header;

  curl = curl_easy_init();
  if (curl)
  {
    /** set basic authentication if present*/
    if(RestClient::user_pass.length()>0){
      curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
      curl_easy_setopt(curl, CURLOPT_USERPWD, RestClient::user_pass.c_str());
    }
    /** set user agent */
    curl_easy_setopt(curl, CURLOPT_USERAGENT, RestClient::user_agent);
    /** set query URL */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    /** set HTTP DELETE METHOD */
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, http_delete);
    /** set callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RestClient::write_callback);
    /** set data object to pass to callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
    /** set the header callback function */
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, RestClient::header_callback);
    /** callback object for headers */
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &ret);
    /** set http headers */
    curl_slist* hlist = NULL;
    for (headermap::const_iterator it = headers.begin(); it != headers.end(); ++it) {
      header = it->first;
      header += ": ";
      header += it->second;
      hlist = curl_slist_append(hlist, header.c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hlist);

    //set timeout
    if (timeout) {
      curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
      curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1); // dont want to get a sig alarm on timeout
    }

    /** perform the actual query */
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
	  if (res == CURLE_OPERATION_TIMEDOUT) {
		ret.code = res;
		ret.body = "Operation Timeout.";
		return ret;
	  }

      ret.body = "Failed to query.";
      ret.code = -1;
      return ret;
    }
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    ret.code = static_cast<int>(http_code);

    curl_slist_free_all(hlist);
    curl_easy_cleanup(curl);
  }

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
 * @brief header callback for libcurl
 *
 * @param data returned (header line)
 * @param size of data
 * @param nmemb memblock
 * @param userdata pointer to user data object to save headr data
 * @return size * nmemb;
 */
size_t RestClient::header_callback(void *data, size_t size, size_t nmemb,
                            void *userdata)
{
  RestClient::response* r;
  r = reinterpret_cast<RestClient::response*>(userdata);
  std::string header(reinterpret_cast<char*>(data), size*nmemb);
  size_t seperator = header.find_first_of(":");
  if ( std::string::npos == seperator ) {
    //roll with non seperated headers...
    trim(header);
    if ( 0 == header.length() ){
      return (size * nmemb); //blank line;
    }
    r->headers[header] = "present";
  } else {
    std::string key = header.substr(0, seperator);
    trim(key);
    std::string value = header.substr(seperator + 1);
    trim (value);
    r->headers[key] = value;
  }

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
  /** get upload struct */
  RestClient::upload_object* u;
  u = reinterpret_cast<RestClient::upload_object*>(userdata);
  /** set correct sizes */
  size_t curl_size = size * nmemb;
  size_t copy_size = (u->length < curl_size) ? u->length : curl_size;
  /** copy data to buffer */
  memcpy(data, u->data, copy_size);
  /** decrement length and increment data pointer */
  u->length -= copy_size;
  u->data += copy_size;
  /** return copied size */
  return copy_size;
}
