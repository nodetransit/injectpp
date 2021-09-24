cmake_minimum_required(VERSION 3.5)
project(injectpp_project)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )

add_subdirectory(./src  injectpp_headers)
add_subdirectory(./test test)

