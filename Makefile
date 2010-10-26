#
# Makefile for restclient-cpp
#

# set compiler and linker
CC = g++
LD = g++

# logfile for HTTP test server
LOGFILE = test_server.log

# binaries and folders
BINDIR = bin
LIBDIR = lib
TEST = $(BINDIR)/test
LIB = $(LIBDIR)/librestclient-cpp.so

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

# lib rule
$(LIB): $(OBJS) $(LIBDIR)
	$(LD) $(LDFLAGS) -shared -soname,$(LIB) -o $(LIB) $(OBJS) $(LIBS)

# compile rule
.cpp.o:
	$(CC) $(CCFLAGS) $(INCLUDE) -c $<  -o $@

$(BINDIR):
	@mkdir -p $(BINDIR)

$(LIBDIR):
	@mkdir -p $(LIBDIR)

# tasks
.PHONY:  clean

clean:
	@rm -rf test/*.o
	@rm -rf source/*.o
	@rm -rf bin
	@rm -rf lib

test: $(TEST)
	@echo Running tests...
	@./bin/test

dynamiclibrary: $(LIB)
