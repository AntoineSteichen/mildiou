cmake_minimum_required(VERSION 3.7.2)
project(weediou)

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-Wall -Wextra")

add_executable(weediou src/main.cpp src/cohorte.cpp src/cohorte.h src/model.cpp
  src/model.h src/default_values.h src/default_values.cpp)
