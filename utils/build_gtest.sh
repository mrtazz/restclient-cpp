#!/bin/sh
cd vendor/googletest-1.14.0
cmake .
which -s gmake && gmake || make
