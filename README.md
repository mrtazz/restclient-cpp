# REST client for C++

## About
This is a simple REST client for C++. It wraps libcurl for HTTP requests.

## Usage
I tried to keep usage close to the [ruby rest-client][]. So the basic usage is:

    restclient::method(url, content-type, params);

Examples:

    #include "restclient.h"

    RestClient::get("http://url.com")
    RestClient::post("http://url.com/post", "text/json", "{"foo": "bla"}")
    RestClient::put("http://url.com/put", "text/json", "{"foo": "bla"}")
    RestClient::del("http://url.com/delete")

The response is of type RestClient::response and has two attributes:

    RestClient::response.code // HTTP response code
    RestClient::response.body // HTTP response body


## Dependencies
- [libcurl][]


[libcurl]: http://curl.haxx.se/libcurl/
[ruby rest-client]: http://github.com/archiloque/rest-client
