# REST client for C++ [![Build Status](https://travis-ci.org/mrtazz/restclient-cpp.svg?branch=master)](https://travis-ci.org/mrtazz/restclient-cpp)

## About
This is a simple REST client for C++. It wraps libcurl for HTTP requests.

## Usage
I tried to keep usage close to the [ruby rest-client][]. So the basic usage is:

    RestClient::method(url, content-type, params);

Examples:

    #include "restclient-cpp/restclient.h"

    RestClient::response r = RestClient::get("http://url.com")
    RestClient::response r = RestClient::post("http://url.com/post", "text/json", "{\"foo\": \"bla\"}")
    RestClient::response r = RestClient::put("http://url.com/put", "text/json", "{\"foo\": \"bla\"}")
    RestClient::response r = RestClient::del("http://url.com/delete")

The response is of type RestClient::response and has three attributes:

    RestClient::response.code // HTTP response code
    RestClient::response.body // HTTP response body
    RestClient::response.headers // HTTP response headers


## Dependencies
- [libcurl][]

## Installation
There are some packages available for Linux on [packagecloud][packagecloud].
Otherwise you can do the regular autotools dance:

```
./autogen.sh
./configure
make install
```

## Contribute
All contributions are highly appreciated. This includes filing issues,
updating documentation and writing code. Please take a look at the
[contributing guidelines][contributing] before so your contribution can be
merged as fast as possible.


[libcurl]: http://curl.haxx.se/libcurl/
[ruby rest-client]: http://github.com/archiloque/rest-client
[gtest]: http://code.google.com/p/googletest/
[packagecloud]: https://packagecloud.io/mrtazz/restclient-cpp
[contributing]: https://github.com/mrtazz/restclient-cpp/blob/master/CONTRIBUTING.md
