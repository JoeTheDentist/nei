#
# libnei qmake project
#

# disabling Qt libraries
QT =
QMAKE_LIBS_QT_ENTRY =

# project configuration
TEMPLATE = lib
TARGET = nei
VERSION = 0.1.0

# compilation options
COMPILER=$$(CXX)
isEmpty(COMPILER) { COMPILER=g++ }
QMAKE_CXX=$${COMPILER}
QMAKE_LINK=$${COMPILER}
QMAKE_CXXFLAGS+=-std=gnu++11

# code
SOURCES = \
    ../src/nei.cpp

HEADERS = \
    ../include/nei.h \
    ../include/debug.h

# test profile
test {
    TEMPLATE = app
    TARGET = libnei_test
    QMAKE_CXXFLAGS += -DBUILD_DIR=\'\"$$PWD\"\'
    SOURCES += ../test/main.cpp \
        ../test/distance_1d.cpp \
        ../test/image_number.cpp
    HEADERS += ../test/common.h \
        ../test/distance_1d.h \
        ../test/image_number.h
    LIBS += -lgtest
}
