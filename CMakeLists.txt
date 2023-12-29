cmake_minimum_required(VERSION 3.10)
project(lexen)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.74)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(test)
