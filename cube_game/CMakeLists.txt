cmake_minimum_required(VERSION 3.28)
project(cube_game LANGUAGES CXX)

add_executable(
	cube_game
	"src/block.cpp"
	"src/cube_game.cpp"
	"src/player.cpp"
	"src/scene.cpp"
)

b_embed(cube_game "assets/cobble.png")

if(CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*")
	target_link_libraries(cube_game PRIVATE -lstdc++exp)
endif()

target_compile_features(cube_game PRIVATE cxx_std_23)
target_compile_options(cube_game PRIVATE ${ZTH_COMPILE_WARNINGS})
set_property(TARGET cube_game PROPERTY COMPILE_WARNING_AS_ERROR On)

target_link_libraries(cube_game PRIVATE zenith)
