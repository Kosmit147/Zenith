cmake_minimum_required(VERSION 3.28)
project(Sandbox LANGUAGES CXX)

add_executable(
	Sandbox
	"src/Sandbox.cpp"
	"src/Scene.cpp"
)

b_embed(Sandbox "assets/container.jpg")
b_embed(Sandbox "assets/emoji.png")
b_embed(Sandbox "assets/wall.jpg")

if(CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*")
	target_link_libraries(Sandbox PRIVATE -lstdc++exp)
endif()

target_compile_features(Sandbox PRIVATE cxx_std_23)
target_compile_options(Sandbox PRIVATE ${ZTH_COMPILE_WARNINGS})
set_property(TARGET Sandbox PROPERTY COMPILE_WARNING_AS_ERROR On)

target_link_libraries(Sandbox PRIVATE Zenith)
