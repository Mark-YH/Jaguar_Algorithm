cmake_minimum_required(VERSION 3.14)
project(JA_Absolute)

set(CMAKE_CXX_STANDARD 17)

add_executable(JA_Absolute main.cpp JA.cpp JA.h Jaguar.cpp Jaguar.h Logger.h Model.cpp Model.h)