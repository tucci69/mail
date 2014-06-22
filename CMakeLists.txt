project(mail)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
set(ASIO_HPP /usr/local/asio/1.10.2/include)
include_directories(${ASIO_HPP})

find_library(OPENSSL_LIB ssl)
find_library(CRYPTO_LIB crypto)

add_subdirectory(src)



