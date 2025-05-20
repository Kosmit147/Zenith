cmake_minimum_required(VERSION 3.28)

function(add_jolt JOLT_ROOT)
    if(NOT DEFINED ZTH_JOLT_BUILD_TYPE)
        message(FATAL_ERROR "ZTH_JOLT_BUILD_TYPE must be defined.")
    endif()

    if(NOT DEFINED ZTH_LINK_WITH_STATIC_CRT)
        message(FATAL_ERROR "ZTH_LINK_WITH_STATIC_CRT must be defined.")
    endif()

    if(ZTH_JOLT_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_BUILD_TYPE "Debug")
    elseif(ZTH_JOLT_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_BUILD_TYPE "Release")
    elseif(ZTH_JOLT_BUILD_TYPE STREQUAL "Distribution")
        set(CMAKE_BUILD_TYPE "Distribution")
    endif()

    message("Building Jolt in ${CMAKE_BUILD_TYPE} mode.")
    
    if(ZTH_LINK_WITH_STATIC_CRT)
        set(USE_STATIC_MSVC_RUNTIME_LIBRARY ON)
    else()
        set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF)
    endif()

    add_subdirectory("${JOLT_ROOT}/Build")
endfunction()
