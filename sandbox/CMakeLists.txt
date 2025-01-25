cmake_minimum_required(VERSION 3.28)
project(sandbox LANGUAGES CXX)

add_executable(
	sandbox
	"src/sandbox.cpp"
	"src/scene.cpp"
)

b_embed(sandbox "assets/wall.jpg")

if(CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*")
	target_link_libraries(sandbox PRIVATE -lstdc++exp)
endif()

target_compile_features(sandbox PRIVATE cxx_std_23)
target_compile_options(sandbox PRIVATE ${ZTH_COMPILE_WARNINGS})
set_property(TARGET sandbox PROPERTY COMPILE_WARNING_AS_ERROR On)

target_link_libraries(sandbox PRIVATE zenith)
