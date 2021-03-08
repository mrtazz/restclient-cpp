/**
 * @file restclient.cpp
 * @brief implementation of the restclient class
 *
 * This just provides static wrappers around the Connection class REST
 * methods. However since I didn't want to have to pull in a whole URI parsing
 * library just now, the Connection constructor is passed an empty string and
 * the full URL is passed to the REST methods. All those methods to is
 * concatenate them anyways. So this should do for now.
 *
 * @author Daniel Schauenberg <d@unwiredcouch.com>
 */

#include "restclient-cpp/restclient.h"

#include <curl/curl.h>
#if __cplusplus >= 201402L
#include <memory>
#endif

#include "restclient-cpp/version.h"
#include "restclient-cpp/connection.h"

/**
 * @brief global init function. Call this before you start any threads.
 */
int RestClient::init() {
  CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
  if (res == CURLE_OK) {
    return 0;
  } else {
    return 1;
  }
}

/**
 * @brief global disable function. Call this before you terminate your
 * program.
 */
void RestClient::disable() {
  curl_global_cleanup();
}

/**
 * @brief HTTP GET method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::Response RestClient::get(const std::string& url) {
  RestClient::Response ret;
  RestClient::Connection conn("");
  ret = conn.get(url);
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
RestClient::Response RestClient::post(const std::string& url,
                                      const std::string& ctype,
                                      const std::string& data) {
  RestClient::Response ret;
  RestClient::Connection conn("");
  conn.AppendHeader("Content-Type", ctype);
  ret = conn.post(url, data);
  return ret;
}

/**
 * @brief HTTP POST Form method
 *
 * @param url to query
 * @param data post form information
 *
 * @return response struct
 */
RestClient::Response RestClient::post(const std::string& url,
                                      const FormData& data) {
  RestClient::Response ret;
  RestClient::Connection conn("");
  ret = conn.post(url, data);
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
RestClient::Response RestClient::put(const std::string& url,
                                     const std::string& ctype,
                                     const std::string& data) {
  RestClient::Response ret;
  RestClient::Connection conn("");
  conn.AppendHeader("Content-Type", ctype);
  ret = conn.put(url, data);
  return ret;
}

/**
 * @brief HTTP PATCH method
 *
 * @param url to query
 * @param ctype content type as string
 * @param data HTTP PATCH body
 *
 * @return response struct
 */
RestClient::Response RestClient::patch(const std::string& url,
                                       const std::string& ctype,
                                       const std::string& data) {
  RestClient::Response ret;
  RestClient::Connection conn("");
  conn.AppendHeader("Content-Type", ctype);
  ret = conn.patch(url, data);
  return ret;
}

/**
 * @brief HTTP DELETE method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::Response RestClient::del(const std::string& url) {
  RestClient::Response ret;
  RestClient::Connection conn("");
  ret = conn.del(url);
  return ret;
}

/**
 * @brief HTTP HEAD method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::Response RestClient::head(const std::string& url) {
  RestClient::Response ret;
  RestClient::Connection conn("");
  ret = conn.head(url);
  return ret;
}

/**
 * @brief HTTP OPTIONS method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::Response RestClient::options(const std::string& url) {
  RestClient::Response ret;
  RestClient::Connection conn("");
  ret = conn.options(url);
  return ret;
}

/**
 * @brief FormData constructor
 */
RestClient::FormData::FormData()
  : formPtr(NULL), lastFormPtr(NULL) {
}

/**
 * @brief FormData destructor
 */
RestClient::FormData::~FormData() {
  // cleanup the formpost chain
  if (this->formPtr) {
    curl_formfree(this->formPtr);
    this->formPtr = NULL;
    this->lastFormPtr = NULL;
  }
}

/**
 * @brief set the name and the value of the HTML "file" form's input
 *
 * @param fieldName name of the "file" input
 * @param fieldValue path to the file to upload
 */
void RestClient::FormData::addFormFile(const std::string& fieldName,
                                           const std::string& fieldValue) {
  curl_formadd(&this->formPtr, &this->lastFormPtr,
               CURLFORM_COPYNAME, fieldName.c_str(),
               CURLFORM_FILE, fieldValue.c_str(),
               CURLFORM_END);
}

/**
 * @brief set the name and the value of an HTML form's input 
 * (other than "file" like "text", "hidden" or "submit")
 *
 * @param fieldName name of the input element
 * @param fieldValue value to be assigned to the input element
 */
void RestClient::FormData::addFormContent(const std::string& fieldName,
                                              const std::string& fieldValue) {
  curl_formadd(&this->formPtr, &this->lastFormPtr,
               CURLFORM_COPYNAME, fieldName.c_str(),
               CURLFORM_COPYCONTENTS, fieldValue.c_str(),
               CURLFORM_END);
}
