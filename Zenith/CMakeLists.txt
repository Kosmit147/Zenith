cmake_minimum_required(VERSION 3.28)
project(Zenith LANGUAGES CXX)

add_library(
	Zenith SHARED
	"src/Core/Application.cpp"
	"src/Platform/Window.cpp"
)

target_include_directories(Zenith PUBLIC "include")
target_compile_features(Zenith PRIVATE cxx_std_23)
target_compile_definitions(Zenith PRIVATE ZTH_BUILD_SO)
target_compile_options(Zenith PRIVATE ${ZTH_COMPILE_WARNINGS})

if(ZTH_PLATFORM_LINUX)
	# hide symbols in Linux shared libraries by default, like on Windows DLLs
	target_compile_options(Zenith PRIVATE -fvisibility=hidden)
endif()

target_precompile_headers(Zenith PRIVATE "src/pch.hpp")
set_property(TARGET Zenith PROPERTY COMPILE_WARNING_AS_ERROR On)

target_link_libraries(Zenith PUBLIC glad)
target_link_libraries(Zenith PUBLIC glfw)
