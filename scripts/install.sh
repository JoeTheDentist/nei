#!/bin/sh

# get gtest
apt-get install libgtest-dev cmake
cd /usr/src/gtest
cmake CMakeLists.txt
make
cp *.a /usr/lib

# get qmake
apt-get install qt4-qmake

# get valgrind
apt-get install valgrind
