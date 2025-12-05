cmake_minimum_required(VERSION 3.5)
project(injectpp VERSION 0.1.0
                 DESCRIPTION "Template IOC Container Library"
                 HOMEPAGE_URL "https://github.com/nodetransit")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(injectpp INTERFACE)
add_library(injectpp::injectpp ALIAS injectpp)
install (TARGETS injectpp
         EXPORT injectpp_Targets
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib
       # PUBLIC_HEADER DESTINATION include
         )
install (FILES "injectpp.hpp"
         DESTINATION include)
install (FILES "injectpp/abstractinstancecontainer.hpp"
               "injectpp/config.hpp"
               "injectpp/injector.hpp"
               "injectpp/injectppcommon.hpp"
               "injectpp/instancecontainer.hpp"
               "injectpp/typeid.hpp"
               "injectpp/typemap.hpp"
         DESTINATION include/injectpp)

target_include_directories(injectpp
                           INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                                     "$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}")

#install(TARGETS injectpp
#        EXPORT injectpp_targets
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

