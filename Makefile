#
# Makefile for restclient-cpp
#

# set compiler and linker
CC = g++
LD = g++

# prefix for installation
PREFIX = .

# logfile for HTTP test server
LOGFILE = test_server.log

# binaries and folders
BINDIR = bin
LIBDIR = lib
TEST = $(BINDIR)/test
LIBNAME = librestclient-cpp.so
STATICNAME = librestclient-cpp.a
LIB = $(LIBDIR)/$(LIBNAME)
STATIC = $(LIBDIR)/$(STATICNAME)

# set library and include paths
INCLUDE =  -I. -I/usr/local/include
TESTLIBS = -lgtest -lcurl
LIBS = -lcurl

# set compiler and linker flags
CCFLAGS = -fPIC -O3 -W -Wall
LDFLAGS = -W -Wall -L/usr/local/lib

# source files
SRCS =  source/restclient.cpp

# test source files
TESTSRCS = $(SRCS)
TESTSRCS += $(wildcard test/test*.cpp)

# dependencies
# object files
OBJS = $(SRCS:.cpp=.o)
TESTOBJS = $(TESTSRCS:.cpp=.o)

# linking rule
$(TEST): $(TESTOBJS) $(BINDIR)
	$(LD) $(LDFLAGS) $(TESTOBJS) -o $(TEST) $(TESTLIBS)

# dynamic lib rule
$(LIB): $(OBJS) $(LIBDIR)
	$(LD) $(LDFLAGS) -shared -o $(LIB) $(OBJS) $(LIBS)

# static lib rule
$(STATIC): $(OBJS) $(LIBDIR)
	ar rcs $(STATIC) $(OBJS)

# compile rule
.cpp.o:
	$(CC) $(CCFLAGS) $(INCLUDE) -c $<  -o $@

$(BINDIR):
	@mkdir -p $(BINDIR)

$(LIBDIR):
	@mkdir -p $(LIBDIR)

# tasks
.PHONY:  clean style

clean:
	@rm -rf test/*.o
	@rm -rf source/*.o
	@rm -rf bin
	@rm -rf lib

test: $(TEST)
	@echo Running tests...
	@./bin/test

style:
	@cppcheck -I . --enable=all --xml source 2>cppcheck.xml

dynamiclibrary: $(LIB)

staticlibrary: $(STATIC)

install: test
	@echo Installing to ${PREFIX}/lib....
	@install -d ${PREFIX}/lib
	@install -m 644 ./lib/${STATICNAME} ${PREFIX}/lib
	@install -m 644 ./lib/${LIBNAME} ${PREFIX}/lib

all: test dynamiclibrary staticlibrary install

