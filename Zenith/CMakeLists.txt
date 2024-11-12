cmake_minimum_required(VERSION 3.28)
project(Zenith LANGUAGES CXX)

add_library(
	Zenith SHARED
	"src/test.cpp"
)

target_include_directories(Zenith PUBLIC "include")
target_compile_features(Zenith PRIVATE cxx_std_23)
target_compile_definitions(Zenith PRIVATE ZTH_BUILD_SO)
target_compile_options(Zenith PRIVATE ${ZTH_COMPILE_WARNINGS})
target_precompile_headers(Zenith PRIVATE "src/pch.hpp")
set_property(TARGET Zenith PROPERTY COMPILE_WARNING_AS_ERROR On)
