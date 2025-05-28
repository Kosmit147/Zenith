cmake_minimum_required(VERSION 3.28)
project(unit_tester LANGUAGES CXX)

include(CTest)
include(Catch)

add_executable(
	unit_tester
	"src/core/cast.cpp"
	"src/math/vector.cpp"
	"src/memory/managed.cpp"
	"src/memory/memory.cpp"
	"src/renderer/shader_preprocessor.cpp"
	"src/stl/string_algorithm.cpp"
	"src/stl/string_hasher.cpp"
	"src/stl/vector.cpp"
	"src/util/defer.cpp"
	"src/util/meta.cpp"
	"src/util/number.cpp"
)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*")
	target_link_libraries(unit_tester PRIVATE -lstdc++exp)
endif()

target_include_directories(unit_tester PRIVATE "src")
target_compile_features(unit_tester PRIVATE cxx_std_23)
target_compile_options(unit_tester PRIVATE ${ZTH_COMPILE_WARNINGS})
target_precompile_headers(unit_tester PRIVATE "src/pch.hpp")
set_property(TARGET unit_tester PROPERTY COMPILE_WARNING_AS_ERROR On)

target_link_libraries(unit_tester PRIVATE Catch2WithMain)
target_link_libraries(unit_tester PRIVATE zenith)

catch_discover_tests(unit_tester)
