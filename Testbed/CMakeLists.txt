cmake_minimum_required(VERSION 3.28)
project(Testbed LANGUAGES CXX)

add_executable(
	Testbed
	"src/containers_scene.cpp"
	"src/embedded.cpp"
	"src/main_scene.cpp"
	"src/testbed.cpp"
)

b_embed(Testbed "assets/cobble_diffuse.png")
b_embed(Testbed "assets/container_diffuse.jpg")
b_embed(Testbed "assets/container2_diffuse.png")
b_embed(Testbed "assets/container2_specular.png")
b_embed(Testbed "assets/emoji_diffuse.png")
b_embed(Testbed "assets/matrix_emission.jpg")
b_embed(Testbed "assets/wall_diffuse.jpg")

if(CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*")
	target_link_libraries(Testbed PRIVATE -lstdc++exp)
endif()

target_compile_features(Testbed PRIVATE cxx_std_23)
target_compile_options(Testbed PRIVATE ${ZTH_COMPILE_WARNINGS})
set_property(TARGET Testbed PROPERTY COMPILE_WARNING_AS_ERROR On)

target_link_libraries(Testbed PRIVATE Zenith)
