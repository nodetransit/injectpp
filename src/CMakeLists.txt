cmake_minimum_required(VERSION 3.5)
project(injectpp VERSION 0.1.0
                 DESCRIPTION "Template IOC Container Library"
                 HOMEPAGE_URL "https://github.com/nodetransit")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

string (REPLACE "." "," PRODUCT_FILE_VERSION ${PROJECT_VERSION})

option (INJECTPP_EXPORT_LIB "Build Shared Libraries." ON)
set (INJECTPP_EXPORT_SHARED OFF)
set (INJECTPP_EXPORT_STATIC OFF)
if (INJECTPP_EXPORT_LIB)
    set (INJECTPP_LIB_TYPE SHARED)
    set (INJECTPP_EXPORT_SHARED ON)
else ()
    set (INJECTPP_LIB_TYPE STATIC)
    set (INJECTPP_EXPORT_STATIC ON)
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (LINUX ON)
endif ()

if (WIN32) # (or LOSE)
    set (LOSE ON)
endif ()

if (APPLE)
    set (BANANA ON)
endif()

if (LOSE)
    if (MSVC)
        set (DLLTOOL OFF)
    else ()
        message (STATUS "Detecting dlltool")
        find_program (DLLTOOL dlltool)
        if (DLLTOOL)
            message (STATUS "  Found dlltool: ${DLLTOOL}")
        else ()
            message (WARNING "dlltool not found. Skipping import library generation.")
        endif ()
    endif ()

    if (MINGW)
        set (CMAKE_RC_COMPILER_INIT windres)
        enable_language (RC)
        set (CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <DEFINES> -o <OBJECT> -i <SOURCE>")
    endif ()

endif ()

set (BINARY_NAME injectpp)
add_library(${BINARY_NAME} ${INJECTPP_LIB_TYPE} "injector.cpp")

configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/injectpp.hpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/injectpp.hpp")

if (LOSE)
    configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/manifest.rc.in" "${CMAKE_CURRENT_SOURCE_DIR}/manifest.rc")
endif ()

if (${INJECTPP_LIB_TYPE} STREQUAL "STATIC")
    set_property(${BINARY_NAME} ${BINARY_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
    add_definitions (-DINJECTPP_LIB_EXPORT=1)
elseif (${INJECTPP_LIB_TYPE} STREQUAL "SHARED")
    if (LOSE)
        set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

        if (MSVC)
            set_target_properties (${BINARY_NAME} PROPERTIES
                                   LINK_FLAGS "${CMAKE_SHARED_LINKER_FLAGS_INIT} \/DYNAMICBASE")
        endif ()

        if (DLLTOOL)
            set_target_properties (${BINARY_NAME} PROPERTIES OUTPUT_NAME "${BINARY_NAME}"
                                                             RUNTIME_OUTPUT_NAME "${BINARY_NAME}"
                                                             LIBRARY_OUTPUT_NAME "${BINARY_NAME}"
                                                             ARCHIVE_OUTPUT_NAME "${BINARY_NAME}"
                                                             LINK_FLAGS "${CMAKE_SHARED_LINKER_FLAGS_INIT} -Wl,--output-def=${BINARY_NAME}.def")

            add_custom_command(TARGET ${BINARY_NAME} POST_BUILD
                               #WORKING_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
                               COMMAND echo "Generating import library"
                               COMMAND "${DLLTOOL}" --dllname "${BINARY_NAME}.dll"
                                                    --input-def "${BINARY_NAME}.def"
                                                    --output-lib "${BINARY_NAME}.lib"
                                                    #todo add --as-flags=--64
                                                    )

        endif ()
    else ()
        set_target_properties (${BINARY_NAME} PROPERTIES OUTPUT_NAME ${BINARY_NAME}
                                                         RUNTIME_OUTPUT_NAME "${BINARY_NAME}"
                                                         LIBRARY_OUTPUT_NAME "${BINARY_NAME}"
                                                         ARCHIVE_OUTPUT_NAME ${BINARY_NAME})
    endif ()

    list (APPEND COMMON_COMPILE_OPTIONS -DINJECTPP_LIB_EXPORT=1)

    set_target_properties(${BINARY_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
    set_target_properties(${BINARY_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
endif ()
target_include_directories(${BINARY_NAME}
                           PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                                  "$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}")

#install(TARGETS ${BINARY_NAME}
#        EXPORT injectpp_targets
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

