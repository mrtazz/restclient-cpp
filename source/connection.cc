/**
 * @file connection.cpp
 * @brief implementation of the connection class
 * @author Daniel Schauenberg <d@unwiredcouch.com>
 */

#include "restclient-cpp/connection.h"

#include <curl/curl.h>

#include <cstring>
#include <string>
#include <iostream>
#include <map>
#include <stdexcept>

#include "restclient-cpp/restclient.h"
#include "restclient-cpp/helpers.h"
#include "restclient-cpp/version.h"

RestClient::Connection::Connection(const std::string baseUrl)
                               : infoStruct(), headerFields() {
  this->curlHandle = curl_easy_init();
  if (!this->curlHandle) {
    throw std::runtime_error("Couldn't initialize curl handle");
  }
  this->baseUrl = baseUrl;
}

RestClient::Connection::~Connection() {
  if (this->curlHandle) {
    curl_easy_cleanup(this->curlHandle);
  }
}

// getters/setters

/**
 * @brief append a header to the internal map
 *
 * @param key for the header field
 * @param value for the header field
 *
 */
void
RestClient::Connection::AppendHeader(const std::string& key,
                              const std::string& value) {
  this->headerFields[key] = value;
}

/**
 * @brief get the user agent to add to the request
 *
 * @return user agent as std::string
 */
std::string
RestClient::Connection::GetUserAgent() {
  std::string prefix;
  if (this->customUserAgent.length() > 0) {
    prefix = this->customUserAgent + " ";
  }
    return std::string(prefix + "restclient-cpp/" + RESTCLIENT_VERSION);
}


/**
 * @brief helper function to get called from the actual request methods to
 * prepare the curlHandle for transfer with generic options, perform the
 * request and record some stats from the last request and then reset the
 * handle with curl_easy_reset to its default state. This will keep things
 * like connections and session ID intact but makes sure you can change
 * parameters on the object for another request.
 *
 * @param uri URI to query
 * @param ret Reference to the Response struct that should be filled
 *
 * @return 0 on success and 1 on error
 */
RestClient::Response
RestClient::Connection::performCurlRequest(const std::string& uri) {
  // init return type
  RestClient::Response ret = {};

  std::string url = std::string(this->baseUrl + uri);
  std::string headerString;
  CURLcode res = CURLE_OK;
  curl_slist* headerList = NULL;

  /** set query URL */
  curl_easy_setopt(this->curlHandle, CURLOPT_URL, url.c_str());
  /** set callback function */
  curl_easy_setopt(this->curlHandle, CURLOPT_WRITEFUNCTION,
                   Helpers::write_callback);
  /** set data object to pass to callback function */
  curl_easy_setopt(this->curlHandle, CURLOPT_WRITEDATA, &ret);
  /** set the header callback function */
  curl_easy_setopt(this->curlHandle, CURLOPT_HEADERFUNCTION,
                   Helpers::header_callback);
  /** callback object for headers */
  curl_easy_setopt(this->curlHandle, CURLOPT_HEADERDATA, &ret);
  /** set http headers */
  for (HeaderFields::const_iterator it = this->headerFields.begin();
      it != this->headerFields.end(); ++it) {
    headerString = it->first;
    headerString += ": ";
    headerString += it->second;
    headerList = curl_slist_append(headerList, headerString.c_str());
  }
  curl_easy_setopt(this->curlHandle, CURLOPT_HTTPHEADER,
      headerList);
  /** set user agent */
  curl_easy_setopt(this->curlHandle, CURLOPT_USERAGENT,
                   this->GetUserAgent().c_str());

  // set timeout
  if (this->timeout) {
    curl_easy_setopt(this->curlHandle, CURLOPT_TIMEOUT, this->timeout);
    // dont want to get a sig alarm on timeout
    curl_easy_setopt(this->curlHandle, CURLOPT_NOSIGNAL, 1);
  }
  res = curl_easy_perform(this->curlHandle);
  if (res != CURLE_OK) {
    if (res == CURLE_OPERATION_TIMEDOUT) {
      ret.code = res;
      ret.body = "Operation Timeout.";
    }

    ret.body = "Failed to query.";
    ret.code = -1;
  }
  int64_t http_code = 0;
  curl_easy_getinfo(this->curlHandle, CURLINFO_RESPONSE_CODE, &http_code);
  ret.code = static_cast<int>(http_code);

  // TODO(mrtazz): get metrics from curl handle
  // free header list
  curl_slist_free_all(headerList);
  // reset curl handle
  curl_easy_reset(this->curlHandle);
  return ret;
}

/**
 * @brief HTTP GET method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::Response
RestClient::Connection::get(const std::string& url) {
  return this->performCurlRequest(url);
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
RestClient::Response
RestClient::Connection::post(const std::string& url,
                             const std::string& data) {
  /** Now specify we want to POST data */
  curl_easy_setopt(this->curlHandle, CURLOPT_POST, 1L);
  /** set post fields */
  curl_easy_setopt(this->curlHandle, CURLOPT_POSTFIELDS, data.c_str());
  curl_easy_setopt(this->curlHandle, CURLOPT_POSTFIELDSIZE, data.size());

  return this->performCurlRequest(url);
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
RestClient::Response
RestClient::Connection::put(const std::string& url,
                            const std::string& data) {
  /** initialize upload object */
  RestClient::Helpers::UploadObject up_obj;
  up_obj.data = data.c_str();
  up_obj.length = data.size();

  /** Now specify we want to PUT data */
  curl_easy_setopt(this->curlHandle, CURLOPT_PUT, 1L);
  curl_easy_setopt(this->curlHandle, CURLOPT_UPLOAD, 1L);
  /** set read callback function */
  curl_easy_setopt(this->curlHandle, CURLOPT_READFUNCTION,
                   RestClient::Helpers::read_callback);
  /** set data object to pass to callback function */
  curl_easy_setopt(this->curlHandle, CURLOPT_READDATA, &up_obj);
  /** set data size */
  curl_easy_setopt(this->curlHandle, CURLOPT_INFILESIZE,
                     static_cast<int64_t>(up_obj.length));

  return this->performCurlRequest(url);
}
/**
 * @brief HTTP DELETE method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::Response
RestClient::Connection::del(const std::string& url) {
  /** we want HTTP DELETE */
  const char* http_delete = "DELETE";

  /** set HTTP DELETE METHOD */
  curl_easy_setopt(this->curlHandle, CURLOPT_CUSTOMREQUEST, http_delete);

  return this->performCurlRequest(url);
}

