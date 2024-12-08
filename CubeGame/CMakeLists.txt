cmake_minimum_required(VERSION 3.28)
project(CubeGame LANGUAGES CXX)

add_executable(
	CubeGame
	"src/Block.cpp"
	"src/CubeGame.cpp"
	"src/Player.cpp"
	"src/Scene.cpp"
)

b_embed(CubeGame "assets/cobble.png")

if(CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*")
	target_link_libraries(CubeGame PRIVATE -lstdc++exp)
endif()

target_link_libraries(CubeGame PRIVATE Zenith)
target_compile_features(CubeGame PRIVATE cxx_std_23)
target_compile_options(CubeGame PRIVATE ${ZTH_COMPILE_WARNINGS})
set_property(TARGET CubeGame PROPERTY COMPILE_WARNING_AS_ERROR On)
