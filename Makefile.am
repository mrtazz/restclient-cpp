ACLOCAL_AMFLAGS=-I m4
CPPFLAGS=-Iinclude -Ivendor/gtest-1.7.0/include
check_PROGRAMS = test-program
pkginclude_HEADERS = include/restclient-cpp/restclient.h include/restclient-cpp/meta.h

test_program_SOURCES = test/test_restclient_delete.cpp test/test_restclient_get.cpp test/test_restclient_post.cpp test/test_restclient_put.cpp test/tests.cpp
test_program_LDADD = .libs/librestclient-cpp.a
test_program_LDFLAGS=-Lvendor/gtest-1.7.0/lib/.libs -lgtest

lib_LTLIBRARIES=librestclient-cpp.la
librestclient_cpp_la_SOURCES=source/restclient.cpp
librestclient_cpp_la_CXXFLAGS=-fPIC
librestclient_cpp_la_LDFLAGS=-version-info 1:0:1
