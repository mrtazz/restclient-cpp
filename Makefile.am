ACLOCAL_AMFLAGS=-I m4
CPPFLAGS=-Iinclude -Ivendor/gtest-1.7.0/include -Ivendor/jsoncpp-0.10.5/dist
check_PROGRAMS = test-program
pkginclude_HEADERS = include/restclient-cpp/restclient.h include/restclient-cpp/meta.h

test_program_SOURCES = vendor/jsoncpp-0.10.5/dist/jsoncpp.cpp test/test_restclient_delete.cpp test/test_restclient_get.cpp test/test_restclient_post.cpp test/test_restclient_put.cpp test/tests.cpp
test_program_LDADD = .libs/librestclient-cpp.a
test_program_LDFLAGS=-Lvendor/gtest-1.7.0/lib/.libs -lgtest

lib_LTLIBRARIES=librestclient-cpp.la
librestclient_cpp_la_SOURCES=source/restclient.cpp
librestclient_cpp_la_CXXFLAGS=-fPIC
librestclient_cpp_la_LDFLAGS=-version-info 1:0:1

.PHONY: check clean-coverage-files coverage-html

test: check
	./test-program

clean-coverage-files:
	find . -name "*.gcda" -print0 | xargs -0 rm

coverage.info:
	./utils/lcov-1.11/bin/lcov --remove coverage.info "/usr*" -o coverage.info

coverage-html: coverage.info
	./utils/lcov-1.11/bin/genhtml coverage.info --output-directory html
