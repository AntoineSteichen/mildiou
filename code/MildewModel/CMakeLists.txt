cmake_minimum_required(VERSION 3.9)
project(MildewModel)

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-Wall -Wextra")

add_executable(MildewModel main.cpp Cohorte.cpp Cohorte.h Model.cpp Model.h DefaultValues.h)
