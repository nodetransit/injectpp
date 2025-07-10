cmake_minimum_required(VERSION 3.5)
project(injectpp_project)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(./src injectpp_headers)
add_subdirectory(./test test)

