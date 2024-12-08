cmake_minimum_required(VERSION 3.28)
include("cmake/warnings.cmake")

project(ZenithEngine LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(B_PRODUCTION_MODE OFF) # for battery-embed
    add_compile_definitions(_DEBUG)
else()
    set(B_PRODUCTION_MODE ON) # for battery-embed
    add_compile_definitions(_RELEASE NDEBUG)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION On)
endif()

get_compile_warnings(ZTH_COMPILE_WARNINGS)

add_subdirectory("dependencies/glad")
add_subdirectory("dependencies/GLFW")
add_subdirectory("dependencies/spdlog")
add_subdirectory("dependencies/GLM")
add_subdirectory("dependencies/embed")
add_subdirectory("dependencies/stb_image")

add_subdirectory("Zenith")
add_subdirectory("Sandbox")
add_subdirectory("Testbed")
add_subdirectory("CubeGame")
