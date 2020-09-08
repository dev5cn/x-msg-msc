cmake_minimum_required(VERSION 2.8.12.2)
#
ADD_DEFINITIONS(-DLINUX)
#
INCLUDE_DIRECTORIES(../libmisc-cpp)
INCLUDE_DIRECTORIES(../libmisc-kafka-c)
INCLUDE_DIRECTORIES(../libxsc-cpp)
INCLUDE_DIRECTORIES(../libxsc-proto-cpp)
INCLUDE_DIRECTORIES(../libx-msg-im-xsc)
INCLUDE_DIRECTORIES(../libx-msg-common-data-struct-cpp)
#
INCLUDE_DIRECTORIES(../libx-msg-msc-pb)
INCLUDE_DIRECTORIES(../libx-msg-msc-core)
INCLUDE_DIRECTORIES(../libx-msg-msc-msg)
INCLUDE_DIRECTORIES($ENV{PROTOBUF}/include)
#
ADD_COMPILE_OPTIONS(-std=c++17 -fPIC -Wall)
ADD_DEFINITIONS(-Wno-builtin-macro-redefined)
SET(CMAKE_CXX_FLAGS $ENV{CXX_FLAGS})
#
LINK_DIRECTORIES(../libmisc-cpp)
LINK_DIRECTORIES(../libmisc-kafka-c)
LINK_DIRECTORIES(../libxsc-cpp)
LINK_DIRECTORIES(../libxsc-proto-cpp)
LINK_DIRECTORIES(../libx-msg-im-xsc)
LINK_DIRECTORIES(../libx-msg-common-data-struct-cpp)
#
LINK_DIRECTORIES(../libx-msg-msc-pb)
LINK_DIRECTORIES(../libx-msg-msc-core)
LINK_DIRECTORIES(../libx-msg-msc-msg)
#
LINK_DIRECTORIES($ENV{PROTOBUF}/lib)
LINK_DIRECTORIES($ENV{KAFKA_DRIVER}/lib)
LINK_DIRECTORIES($ENV{LIB_UUID}/lib)
LINK_DIRECTORIES($ENV{CYRUS_SASL_LIB})
LINK_DIRECTORIES($ENV{OPENSSL_LIB})
LINK_DIRECTORIES($ENV{Z_LIB})
LINK_DIRECTORIES($ENV{LIBSTDCXX})
#
LINK_LIBRARIES(x-msg-msc-msg)
LINK_LIBRARIES(x-msg-msc-core)
LINK_LIBRARIES(x-msg-msc-pb)
#
LINK_LIBRARIES(x-msg-common-data-struct-cpp)
LINK_LIBRARIES(x-msg-im-xsc)
LINK_LIBRARIES(xsc-proto-cpp)
LINK_LIBRARIES(xsc-cpp)
LINK_LIBRARIES(misc-kafka-c)
LINK_LIBRARIES(misc-cpp)
LINK_LIBRARIES(librdkafka.a libsasl2.a libssl.a libcrypto.a libz.a dl)
LINK_LIBRARIES(libprotobuf.a)
LINK_LIBRARIES(libuuid.a)
LINK_LIBRARIES(libstdc++.a)
LINK_LIBRARIES(pthread)
#
FILE(GLOB SRC0 "./*.cpp")
#
PROJECT(x-msg-msc)
#
ADD_EXECUTABLE(x-msg-msc ${SRC0})
INCLUDE(./__file__.cmake)
REDEFINE_FILE_MACRO(x-msg-msc)

