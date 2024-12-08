cmake_minimum_required(VERSION 3.28)
project(Zenith LANGUAGES CXX)

add_library(
	Zenith STATIC
	"src/Core/Application.cpp"
	"src/Core/Exception.cpp"
	"src/Core/main.cpp"
	"src/Core/SceneManager.cpp"
	"src/Core/SystemManager.cpp"
	"src/Core/Transformable.cpp"
	"src/Graphics/Shapes/CubeShape.cpp"
	"src/Graphics/Shapes/Shape.cpp"
	"src/Graphics/Camera.cpp"
	"src/Graphics/Material.cpp"
	"src/Graphics/Mesh.cpp"
	"src/Graphics/Meshes.cpp"
	"src/Graphics/Renderer.cpp"
	"src/Graphics/Shaders.cpp"
	"src/Logging/Logger.cpp"
	"src/Platform/OpenGl/GlBuffer.cpp"
	"src/Platform/OpenGl/GlContext.cpp"
	"src/Platform/OpenGl/GlDebug.cpp"
	"src/Platform/OpenGl/Shader.cpp"
	"src/Platform/OpenGl/Texture.cpp"
	"src/Platform/OpenGl/VertexArray.cpp"
	"src/Platform/Event.cpp"
	"src/Platform/EventQueue.cpp"
	"src/Platform/Input.cpp"
	"src/Platform/Key.cpp"
	"src/Platform/Mouse.cpp"
	"src/Platform/Window.cpp"
	"src/Time/Time.cpp"
)

b_embed(Zenith "src/Graphics/Shaders/fallback.vert")
b_embed(Zenith "src/Graphics/Shaders/fallback.frag")
b_embed(Zenith "src/Graphics/Shaders/texture.vert")
b_embed(Zenith "src/Graphics/Shaders/texture.frag")

target_include_directories(Zenith PUBLIC "include")
target_compile_features(Zenith PRIVATE cxx_std_23)
target_compile_options(Zenith PRIVATE ${ZTH_COMPILE_WARNINGS})
target_precompile_headers(Zenith PRIVATE "src/pch.hpp")
set_property(TARGET Zenith PROPERTY COMPILE_WARNING_AS_ERROR On)

target_compile_definitions(Zenith PRIVATE GLM_ENABLE_EXPERIMENTAL)

target_link_libraries(Zenith PUBLIC glad)
target_link_libraries(Zenith PUBLIC glfw)
target_link_libraries(Zenith PUBLIC spdlog $<$<BOOL:${MINGW}>:ws2_32>)
target_link_libraries(Zenith PUBLIC glm)
target_link_libraries(Zenith PUBLIC stb_image)
