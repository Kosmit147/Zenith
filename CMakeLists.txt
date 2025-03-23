cmake_minimum_required(VERSION 3.28)
include("cmake/warnings.cmake")

project(zenith_engine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

option(ZTH_DIST_BUILD OFF)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(B_PRODUCTION_MODE OFF) # For battery-embed.
    add_compile_definitions(ZTH_DEBUG_BUILD)
elseif(CMAKE_BUILD_TYPE MATCHES "Release"
        OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo"
        OR CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
    set(B_PRODUCTION_MODE OFF) # For battery-embed.
    add_compile_definitions(NDEBUG)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION On)
endif()

if(ZTH_DIST_BUILD)
    set(B_PRODUCTION_MODE ON) # For battery-embed.
    add_compile_definitions(ZTH_DIST_BUILD)
    add_compile_definitions(ENTT_DISABLE_ASSERT)
else()
    message(WARNING "ZTH_DIST_BUILD is off. Enable ZTH_DIST_BUILD option to strip all debugging features when deploying your application.")
endif()

get_compile_warnings(ZTH_COMPILE_WARNINGS)

if(MSVC)
	set(ENTT_INCLUDE_NATVIS TRUE)
endif()

add_subdirectory("dependencies/Catch2")
add_subdirectory("dependencies/embed")
add_subdirectory("dependencies/entt")
add_subdirectory("dependencies/glad")
add_subdirectory("dependencies/glfw")
add_subdirectory("dependencies/glm")
add_subdirectory("dependencies/spdlog")
add_subdirectory("dependencies/stb_image")

add_subdirectory("zenith")
add_subdirectory("sandbox")
add_subdirectory("testbed")
add_subdirectory("cube_game")
add_subdirectory("unit_tester")

enable_testing()
