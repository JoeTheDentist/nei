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
COMPILER=$$(CXX)
isEmpty(COMPILER) { COMPILER=g++ }
QMAKE_CXX=$${COMPILER}
QMAKE_LINK=$${COMPILER}

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
    SOURCES += ../test/main.cpp
    LIBS += -lpthread -lgtest
}
