cmake_minimum_required(VERSION 3.28)
project(unit_tester LANGUAGES CXX)

include(CTest)
include(Catch)

add_executable(
	unit_tester
	"src/defer.cpp"
	"src/inplace_vector.cpp"
	"src/shader_preprocessor.cpp"
	"src/string.cpp"
	"src/string_hasher.cpp"
)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*")
	target_link_libraries(unit_tester PRIVATE -lstdc++exp)
endif()

target_compile_features(unit_tester PRIVATE cxx_std_23)
target_compile_options(unit_tester PRIVATE ${ZTH_COMPILE_WARNINGS})
target_precompile_headers(unit_tester PRIVATE "src/pch.hpp")
set_property(TARGET unit_tester PROPERTY COMPILE_WARNING_AS_ERROR On)

target_link_libraries(unit_tester PRIVATE Catch2WithMain)
target_link_libraries(unit_tester PRIVATE zenith_core)

catch_discover_tests(unit_tester)
