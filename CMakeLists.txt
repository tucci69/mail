project(mail)
cmake_minimum_required(VERSION 2.6)

set(ASIO_HPP /usr/local/asio/1.10.2/include)
include_directories(${ASIO_HPP})

find_package(OpenSSL REQUIRED)
find_library(OPENSSL_LIB ssl)
find_library(CRYPTO_LIB crypto)

set(COMPILE_FLAGS "-std=c++11 -pthread")
set(LINK_FLAGS "-pthread")

set(CMAKE_CXX_FLAGS "-std=c++0x")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_subdirectory(src)



