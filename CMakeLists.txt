cmake_minimum_required(VERSION 3.5)
project(injectpp_project)

add_subdirectory(./src injectpp_headers)
add_subdirectory(./test test)

