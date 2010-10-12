# REST client for C++

## About
This is a simple REST client for C++. It wraps libcurl for HTTP requests
and uses jsoncpp for parsing JSON responses.

## Usage
I tried to keep usage close to the [ruby rest-client][]. So the basic usage is:

    restclient::method(url, content-type, params);

Examples:

    #include "restclient.h"

    RestClient::get("http://url.com")
    RestClient::post("http://url.com/post", "text/json", "{"foo": "bla"}")
    RestClient::put("http://url.com/put", "text/json", "{"foo": "bla"}")
    RestClient::del("http://url.com/delete")

## Dependencies
- [libcurl][]
- [jsoncpp][]


[libcurl]: http://curl.haxx.se/libcurl/
[jsoncpp]: http://jsoncpp.sourceforge.net/
[ruby rest-client]: http://github.com/archiloque/rest-client
