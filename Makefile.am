ACLOCAL_AMFLAGS=-I m4
CPPFLAGS=-Iinclude
check_PROGRAMS = test-program
pkginclude_HEADERS = include/restclient-cpp/restclient.h include/restclient-cpp/version.h
BUILT_SOURCES = include/restclient-cpp/version.h

test_program_SOURCES = vendor/jsoncpp-0.10.5/dist/jsoncpp.cpp test/test_restclient_delete.cpp test/test_restclient_get.cpp test/test_restclient_post.cpp test/test_restclient_put.cpp test/tests.cpp
test_program_LDADD = .libs/librestclient-cpp.a
test_program_LDFLAGS=-Lvendor/gtest-1.7.0/lib/.libs -lgtest
test_program_CPPFLAGS=-Iinclude -Ivendor/gtest-1.7.0/include -Ivendor/jsoncpp-0.10.5/dist

lib_LTLIBRARIES=librestclient-cpp.la
librestclient_cpp_la_SOURCES=source/restclient.cpp
librestclient_cpp_la_CXXFLAGS=-fPIC
librestclient_cpp_la_LDFLAGS=-version-info 1:0:1

.PHONY: test check clean-coverage-files coverage-html docs clean-docs deploy-docs doxygen jekyll packages rpm deb include/restclient-cpp/version.h deploy-packages

test: check
	./test-program

valgrind: check
	valgrind --leak-check=full --error-exitcode=0 ./test-program

clean-local:
	find . -name "*.gcda" -print0 | xargs -0 rm

coverage.info: test
	lcov --compat-libtool --capture --directory source --output-file coverage.info
	lcov --compat-libtool --remove coverage.info "/usr*" -o coverage.info

coverage-html: coverage.info
	genhtml -t "restclient-cpp coverage report" --legend --show-details coverage.info --output-directory html

local-install:
	$(MAKE) install PREFIX=usr

NAME=restclient-cpp
VERSION = $(shell git describe --tags --always --dirty)
BUILDER = $(shell echo "`git config user.name` <`git config user.email`>")
PKG_RELEASE ?= 1
PROJECT_URL="https://github.com/mrtazz/$(NAME)"
FPM_FLAGS= --name $(NAME) --version $(VERSION) --iteration $(PKG_RELEASE) \
           --epoch 1 --license MIT --maintainer "$(BUILDER)" --url $(PROJECT_URL) \
           --vendor mrtazz --description "C++ client for making HTTP/REST requests" \
           --depends curl usr

rpm:
	  fpm -t rpm -s dir $(FPM_FLAGS)

deb:
	  fpm -t deb -s dir $(FPM_FLAGS)

packages: local-install rpm deb

deploy-packages: packages
	package_cloud push mrtazz/$(NAME)/el/7 *.rpm
	package_cloud push mrtazz/$(NAME)/debian/wheezy *.deb
	package_cloud push mrtazz/$(NAME)/ubuntu/trusty *.deb

include/restclient-cpp/version.h:
	m4 -I m4 --define=M4_RESTCLIENT_VERSION=$(VERSION) version.h.m4 > $@


GAUGES_CODE="539c6c8de32bb442640005f3"

doxygen:
	doxygen doxygen.config

jekyll:
	echo "gaugesid: $(GAUGES_CODE)" > docs/_config.yml
	echo "projecturl: $(PROJECT_URL)" >> docs/_config.yml
	echo "basesite: http://www.unwiredcouch.com" >> docs/_config.yml
	echo "markdown: redcarpet" >> docs/_config.yml
	echo "---" > docs/index.md
	echo "layout: project" >> docs/index.md
	echo "title: $(NAME)" >> docs/index.md
	echo "---" >> docs/index.md
	cat README.md >> docs/index.md

docs: doxygen jekyll

clean-docs:
	rm -rf ./docs

deploy-docs: docs
	@cd docs && git init && git remote add upstream "https://${GH_TOKEN}@github.com/mrtazz/$(NAME).git" && \
	git submodule add https://github.com/mrtazz/jekyll-layouts.git ./_layouts && \
	git submodule update --init && \
	git fetch upstream && git reset upstream/gh-pages && \
	git config user.name 'Daniel Schauenberg' && git config user.email d@unwiredcouch.com && \
	touch . && git add -A . && \
	git commit -m "rebuild pages at $(VERSION)" && \
	git push -q upstream HEAD:gh-pages

