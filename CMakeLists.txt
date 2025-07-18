cmake_minimum_required(VERSION 3.28)
include("cmake/warnings.cmake")

project(zenith_engine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if(NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS OFF)
endif()

option(ZTH_DEVELOPER ${PROJECT_IS_TOP_LEVEL})

if(NOT CMAKE_BUILD_TYPE)
    message(WARNING "No build type set. Defaulting to Release.")
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(MSVC)
    # Replace /Ob1 with /Ob2 in RelWithDebInfo builds to enable full inlining.

    string(REPLACE "/Ob1" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Ob2")

    string(REPLACE "/Ob1" "" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /Ob2")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ZTH_ASSERTIONS_DEFAULT ON)
    set(ZTH_GL_DEBUG_DEFAULT ON)
    set(ZTH_DEBUG_LOGGING_DEFAULT ON)
    set(ZTH_PROFILER_DEFAULT OFF)

    set(B_PRODUCTION_MODE OFF) # For battery-embed.
elseif(CMAKE_BUILD_TYPE STREQUAL "Release"
           OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"
           OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

    set(ZTH_ASSERTIONS_DEFAULT OFF)
    set(ZTH_GL_DEBUG_DEFAULT OFF)
    set(ZTH_DEBUG_LOGGING_DEFAULT OFF)
    set(ZTH_PROFILER_DEFAULT OFF)

    set(B_PRODUCTION_MODE ON) # For battery-embed.
endif()

set(ZTH_ASSERTIONS ${ZTH_ASSERTIONS_DEFAULT} CACHE BOOL "Enable assertions and debug breaks")
set(ZTH_GL_DEBUG ${ZTH_GL_DEBUG_DEFAULT} CACHE BOOL "Enable OpenGL debugging features")
set(ZTH_DEBUG_LOGGING ${ZTH_DEBUG_LOGGING_DEFAULT} CACHE BOOL "Enable debug logging")
set(ZTH_PROFILER ${ZTH_PROFILER_DEFAULT} CACHE BOOL "Enable profiler")

get_compile_warnings(ZTH_COMPILE_WARNINGS)

# Have to overwrite Assimp's really silly defaults.
if(NOT DEFINED ASSIMP_INSTALL)
    set(ASSIMP_INSTALL OFF)
endif()

if(NOT DEFINED ASSIMP_BUILD_TESTS)
    set(ASSIMP_BUILD_TESTS OFF)
endif()

if(NOT DEFINED ASSIMP_INJECT_DEBUG_POSTFIX)
    set(ASSIMP_INJECT_DEBUG_POSTFIX OFF)
endif()

if(NOT DEFINED CATCH_INSTALL_DOCS)
    set(CATCH_INSTALL_DOCS OFF)
endif()

if(MSVC)
    set(ENTT_INCLUDE_NATVIS TRUE)
endif()

add_subdirectory("dependencies/assimp")
add_subdirectory("dependencies/embed")
add_subdirectory("dependencies/entt")
add_subdirectory("dependencies/glad")
add_subdirectory("dependencies/glfw")
add_subdirectory("dependencies/glm")
add_subdirectory("dependencies/spdlog")
add_subdirectory("dependencies/stb_image")

add_subdirectory("zenith")

if(ZTH_DEVELOPER)
    add_subdirectory("dependencies/Catch2")
    add_subdirectory("testbed")
    add_subdirectory("unit_tester")
    enable_testing()
endif()
