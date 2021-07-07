project(injectpp_test)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#---------------------------------------------------------------------

option (DEBUG "Compile with debug symbols." OFF)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set (DEBUG ON)
endif ()
if (NOT DEBUG)
    set (RELEASE ON)
    set (NDEBUG ON)
else ()
    set (INJECTPP_DEBUG ON)
    set (DEBUG ON)
    set (_DEBUG ON)
    set (RELEASE OFF)
endif ()

option (VERBOSE "Verbose console output." OFF)
if (VERBOSE)
    set (CMAKE_VERBOSE_MAKEFILE ON)
endif ()

#---------------------------------------------------------------------

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (LINUX ON)
endif()


if (WIN32)
    # operating system for the spoiled and the losers
    set (LOSER ON)
endif ()
# no support for the weirdos using APPLE

#---------------------------------------------------------------------


file(GLOB SOURCE_FILES "main.cpp")

if (MSVC)
    #file(GLOB HEADER_FILES "test/xxxx.hpp")
else ()
    set (HEADER_FILES "")
endif ()

add_executable(injectpp_test ${SOURCE_FILES})

#---------------------------------------------------------------------

if (NOT MSVC)
    target_compile_options (injectpp_test PRIVATE "-std=c++11")
    add_definitions("--pedantic")

    target_compile_options (injectpp_test PRIVATE "-Wno-missing-braces"
                                                  "-Wall")

    if (DEBUG)
        target_compile_options (injectpp_test PRIVATE #"-Wno-bool-compare"
                                                      #"-Wno-incompatible-pointer-types"
                                                      #"-Wno-unused-variable"
                                                      "-Wno-unused-function"
                                                      "-Wno-format")
    endif ()

endif ()

if (DEBUG)
    message (STATUS "Compiling with DEBUG symbols")

    add_definitions (-DINJECTPP_DEBUG_DEBUG)

    if (NOT MSVC)
        add_definitions (-g
                         #-Wall
                         #-Werror
                         #-Wfatal-errors
                         -fdiagnostics-show-option
                         -fmax-errors=3)
    else ()
        add_definitions ("-ZI")
    endif ()
else ()
    if (NOT MSVC)
        target_compile_options (injectpp_test PRIVATE "-O3"
                                                      "-s")
    else ()
        target_compile_options (injectpp_test PRIVATE "-Ox"
                                                      "-GF"
                                                      "-GL")
    endif ()
endif ()


#---------------------------------------------------------------------

target_link_libraries(injectpp_test injectpp)

