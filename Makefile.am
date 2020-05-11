ACLOCAL_AMFLAGS=-I m4
CPPFLAGS=-I${top_srcdir}/include
check_PROGRAMS = test-program
pkginclude_HEADERS = include/restclient-cpp/restclient.h include/restclient-cpp/version.h include/restclient-cpp/connection.h include/restclient-cpp/helpers.h
BUILT_SOURCES = include/restclient-cpp/version.h

test_program_SOURCES = vendor/jsoncpp-0.10.5/dist/jsoncpp.cpp test/tests.cpp test/test_helpers.cc test/test_restclient.cc test/test_connection.cc
test_program_LDADD = .libs/librestclient-cpp.a
test_program_LDFLAGS=-Lvendor/gtest-1.7.0/lib/.libs -lgtest
test_program_CPPFLAGS=-std=c++11 -Iinclude -Ivendor/gtest-1.7.0/include -Ivendor/jsoncpp-0.10.5/dist

lib_LTLIBRARIES=librestclient-cpp.la
librestclient_cpp_la_SOURCES=source/restclient.cc source/connection.cc source/helpers.cc
librestclient_cpp_la_CXXFLAGS=-fPIC -std=c++11
librestclient_cpp_la_LDFLAGS=-version-info 2:1:1

dist_doc_DATA = README.md

.PHONY: test check clean-coverage-files coverage-html include/restclient-cpp/version.h lint ci docker-services clean-docker-services

include/restclient-cpp/version.h:
	m4 -I ${top_srcdir}/m4 -DM4_RESTCLIENT_VERSION=$(PACKAGE_VERSION) version.h.m4 > ${top_srcdir}/$@ 
 


test: check docker-services
	./test-program

valgrind: check
	valgrind --leak-check=full --error-exitcode=1 ./test-program

lint:
	cpplint --filter=-legal/copyright --root=$(CURDIR) include/restclient-cpp/*.h source/*.cc

docker-services:
	[ -n "$$(docker ps --quiet --filter name=restclient-cpp-httpbin)" ] || \
		docker run --detach -p 8998:80 --name restclient-cpp-httpbin kennethreitz/httpbin
	[ -n "$$(docker ps --quiet --filter name=restclient-cpp-squid)" ] || \
		docker run --detach -p 3128:3128 --name restclient-cpp-squid \
		--volume "$(CURDIR)/test/squid.conf:/etc/squid/squid.conf:ro" sameersbn/squid:3.5.27-2
	docker ps --all --filter 'name=^restclient-cpp-'

clean-docker-services:
	docker rm --force restclient-cpp-httpbin 2>/dev/null || true
	docker rm --force restclient-cpp-squid 2>/dev/null || true

ci: lint test valgrind

clean-local:
	find . -name "*.gcda" -print0 | xargs -0 rm

coverage.info: test
	lcov --compat-libtool --capture --directory source --output-file coverage.info
	lcov --compat-libtool --remove coverage.info "/usr*" -o coverage.info

coverage-html: coverage.info
	genhtml -t "restclient-cpp coverage report" --legend --show-details coverage.info --output-directory html

# include other make targets in the main Makefile
include $(srcdir)/Makefile.packaging.mk
include $(srcdir)/Makefile.docs.mk

