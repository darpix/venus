cmake_minimum_required(VERSION 2.6)
project(venus)

add_executable(venus main.cpp)

install(TARGETS venus RUNTIME DESTINATION bin)
