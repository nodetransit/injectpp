cmake_minimum_required(VERSION 3.5)
project(injectpp VERSION 0.1.0
                 DESCRIPTION "Template IOC Container Library"
                 HOMEPAGE_URL "https://github.com/nodetransit")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(injectpp INTERFACE)

target_include_directories(injectpp
                           INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                                     "$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}")

#install(TARGETS injectpp
#        EXPORT injectpp_targets
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

