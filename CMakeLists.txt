cmake_minimum_required(VERSION 2.6)
project(venus)

add_executable(venus test.cpp src/vwindow.cpp)

install(TARGETS venus RUNTIME DESTINATION bin)
