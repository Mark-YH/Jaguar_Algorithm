cmake_minimum_required(VERSION 3.14)
project(JaguarAlgorithm)

set(CMAKE_CXX_STANDARD 17)

add_executable(JaguarAlgorithm main.cpp JA.cpp JA.h Jaguar.cpp Jaguar.h Logger.h Model.cpp Model.h)