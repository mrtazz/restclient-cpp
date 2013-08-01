#!/bin/sh
cd vendor/gtest-1.6.0
autoreconf -i
./configure
make
