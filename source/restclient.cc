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
#include "restclient-cpp/version.h"


/**
 * @brief HTTP GET method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::response RestClient::get(const std::string& url) {
  Connection *conn = new Connection("");
  return conn->get(url);
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
                                      const std::string& data) {
  Connection *conn = new Connection("");
  return conn->post(url, ctype, data);
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
                                     const std::string& data) {
  Connection *conn = new Connection("");
  return conn->put(url, ctype, data);
}

/**
 * @brief HTTP DELETE method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::response RestClient::del(const std::string& url) {
  Connection *conn = new Connection("");
  return conn->del(url);
}

