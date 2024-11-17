cmake_minimum_required(VERSION 3.28)
project(Zenith LANGUAGES CXX)

add_library(
	Zenith STATIC
	"src/Core/Application.cpp"
	"src/Core/Engine.cpp"
	"src/Core/Exception.cpp"
	"src/Logging/Logger.cpp"
	"src/Platform/Window.cpp"
)

target_include_directories(Zenith PUBLIC "include")
target_compile_features(Zenith PRIVATE cxx_std_23)
target_compile_options(Zenith PRIVATE ${ZTH_COMPILE_WARNINGS})
target_precompile_headers(Zenith PRIVATE "src/pch.hpp")
set_property(TARGET Zenith PROPERTY COMPILE_WARNING_AS_ERROR On)

target_link_libraries(Zenith PUBLIC glad)
target_link_libraries(Zenith PUBLIC glfw)
target_link_libraries(Zenith PUBLIC spdlog $<$<BOOL:${MINGW}>:ws2_32>)
