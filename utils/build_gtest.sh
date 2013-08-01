#!/bin/sh
wget http://googletest.googlecode.com/files/gtest-1.6.0.zip
unzip gtest-1.6.0.zip
cd gtest-1.6.0
./configure
make
sudo cp -a include/gtest /usr/include
sudo cp -a lib/.libs/* /usr/lib/
sudo ldconfig -v | grep gtest
