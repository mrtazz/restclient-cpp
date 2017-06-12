/**
 * @file restclient.h
 * @brief libcurl wrapper for REST calls
 * @author Daniel Schauenberg <d@unwiredcouch.com>
 * @version
 * @date 2010-10-11
 */

#ifndef INCLUDE_RESTCLIENT_CPP_RESTCLIENT_H_
#define INCLUDE_RESTCLIENT_CPP_RESTCLIENT_H_

#include <curl/curl.h>
#include <string>
#include <map>
#include <cstdlib>

#include "restclient-cpp/helpers.h"
#include "restclient-cpp/version.h"

/**
 * @brief namespace for all RestClient definitions
 */
namespace RestClient {

/**
  * public data definitions
  */
typedef std::map<std::string, std::string> HeaderFields;

/** @struct Response
  *  @brief This structure represents the HTTP response data
  *  @var Response::code
  *  Member 'code' contains the HTTP response code
  *  @var Response::body
  *  Member 'body' contains the HTTP response body
  *  @var Response::headers
  *  Member 'headers' contains the HTTP response headers
  */
typedef struct {
  int code;
  std::string body;
  HeaderFields headers;
} Response;

/** @class PostFormInfo
  * @brief This class represents the form information to send on
  * POST Form requests
  */
class PostFormInfo {
  struct curl_httppost* formPtr;
  struct curl_httppost* lastFormPtr;
 public:
  PostFormInfo();
  ~PostFormInfo();
  /* Fill in the file upload field */
  void addFormFile(const std::string& fieldName,
                   const std::string& fieldValue);
  /* Fill in the filename or the submit field */
  void addFormContent(const std::string& fieldName,
                        const std::string& fieldValue);
  /* Get Form pointer */
  struct curl_httppost* GetFormPtr() const { return formPtr; }
};

// init and disable functions
int init();
void disable();

/**
  * public methods for the simple API. These don't allow a lot of
  * configuration but are meant for simple HTTP calls.
  *
  */
Response get(const std::string& url);
Response post(const std::string& url,
              const std::string& content_type,
              const std::string& data);
Response postForm(const std::string& url,
                  const PostFormInfo& data);
Response put(const std::string& url,
              const std::string& content_type,
              const std::string& data);
Response del(const std::string& url);
Response head(const std::string& url);

}  // namespace RestClient

#endif  // INCLUDE_RESTCLIENT_CPP_RESTCLIENT_H_
