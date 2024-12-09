cmake_minimum_required(VERSION 3.28)
project(Testbed LANGUAGES CXX)

add_executable(
	Testbed
	"src/Testbed.cpp"
	"src/TransformTest.cpp"
)

b_embed(Testbed "assets/container.jpg")
b_embed(Testbed "assets/emoji.png")
b_embed(Testbed "assets/wall.jpg")

if(CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*")
	target_link_libraries(Testbed PRIVATE -lstdc++exp)
endif()

target_link_libraries(Testbed PRIVATE Zenith)
target_compile_features(Testbed PRIVATE cxx_std_23)
target_compile_options(Testbed PRIVATE ${ZTH_COMPILE_WARNINGS})
set_property(TARGET Testbed PROPERTY COMPILE_WARNING_AS_ERROR On)
