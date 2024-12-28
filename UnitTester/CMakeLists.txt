cmake_minimum_required(VERSION 3.28)
project(UnitTester LANGUAGES CXX)

include(CTest)
include(Catch)

add_executable(
	UnitTester
	"src/InPlaceVector.cpp"
	"src/ScopeGuard.cpp"
)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*")
	target_link_libraries(Sandbox PRIVATE -lstdc++exp)
endif()

target_compile_features(UnitTester PRIVATE cxx_std_23)
target_compile_options(UnitTester PRIVATE ${ZTH_COMPILE_WARNINGS})
set_property(TARGET UnitTester PROPERTY COMPILE_WARNING_AS_ERROR On)

target_link_libraries(UnitTester PRIVATE Catch2WithMain)
target_link_libraries(UnitTester PRIVATE ZenithNoMain)

catch_discover_tests(UnitTester)
