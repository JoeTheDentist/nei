#!/bin/sh

apt-get install libgtest-dev cmake qt5-qmake
cd /usr/src/gtest
cmake CMakeLists.txt
make
cp *.a /usr/lib

