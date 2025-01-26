cmake_minimum_required(VERSION 3.28)
project(testbed LANGUAGES CXX)

add_executable(
	testbed
	"src/containers_scene.cpp"
	"src/embedded.cpp"
	"src/main_scene.cpp"
	"src/testbed.cpp"
)

b_embed(testbed "assets/cobble_diffuse.png")
b_embed(testbed "assets/container_diffuse.jpg")
b_embed(testbed "assets/container2_diffuse.png")
b_embed(testbed "assets/container2_specular.png")
b_embed(testbed "assets/emoji_diffuse.png")
b_embed(testbed "assets/matrix_emission.jpg")
b_embed(testbed "assets/wall_diffuse.jpg")

if(CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*")
	target_link_libraries(testbed PRIVATE -lstdc++exp)
endif()

target_compile_features(testbed PRIVATE cxx_std_23)
target_compile_options(testbed PRIVATE ${ZTH_COMPILE_WARNINGS})
target_precompile_headers(testbed PRIVATE "src/pch.hpp")
set_property(TARGET testbed PROPERTY COMPILE_WARNING_AS_ERROR On)

target_link_libraries(testbed PRIVATE zenith)
