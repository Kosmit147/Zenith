cmake_minimum_required(VERSION 3.28)
project(Zenith VERSION 0.1.0 LANGUAGES CXX)

add_library(
	ZenithCore STATIC
	"src/Core/Application.cpp"
	"src/Core/AssetManager.cpp"
	"src/Core/CameraController.cpp"
	"src/Core/Exception.cpp"
	"src/Core/SceneManager.cpp"
	"src/Core/SystemManager.cpp"
	"src/Core/Transformable.cpp"
	"src/Debug/Ui/Ui.cpp"
	"src/Filesystem/File.cpp"
	"src/Graphics/Shapes/CubeShape.cpp"
	"src/Graphics/Shapes/PyramidShape.cpp"
	"src/Graphics/Shapes/Shape.cpp"
	"src/Graphics/Shapes/SphereShape.cpp"
	"src/Graphics/Camera.cpp"
	"src/Graphics/DrawCommand.cpp"
	"src/Graphics/ImGuiRenderer.cpp"
	"src/Graphics/Materials.cpp"
	"src/Graphics/Mesh.cpp"
	"src/Graphics/Meshes.cpp"
	"src/Graphics/Renderer.cpp"
	"src/Graphics/Shaders.cpp"
	"src/Logging/Logger.cpp"
	"src/Math/Matrix.cpp"
	"src/Math/Quaternion.cpp"
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
	"src/Platform/ShaderPreprocessor.cpp"
	"src/Platform/Window.cpp"
	"src/Time/Time.cpp"
	"src/Utility/String.cpp"
)

b_embed(ZenithCore "src/Graphics/Shaders/test.glsl")
b_embed(ZenithCore "src/Graphics/Shaders/zth_defines.glsl")
b_embed(ZenithCore "src/Graphics/Shaders/zth_fallback.vert")
b_embed(ZenithCore "src/Graphics/Shaders/zth_fallback.frag")
b_embed(ZenithCore "src/Graphics/Shaders/zth_flat_color.vert")
b_embed(ZenithCore "src/Graphics/Shaders/zth_flat_color.frag")
b_embed(ZenithCore "src/Graphics/Shaders/zth_standard.vert")
b_embed(ZenithCore "src/Graphics/Shaders/zth_standard.frag")

target_include_directories(ZenithCore PUBLIC "include")
target_compile_features(ZenithCore PRIVATE cxx_std_23)
target_compile_options(ZenithCore PRIVATE ${ZTH_COMPILE_WARNINGS})
target_precompile_headers(ZenithCore PRIVATE "src/pch.hpp")
set_property(TARGET ZenithCore PROPERTY COMPILE_WARNING_AS_ERROR On)

# GLM_ENABLE_EXPERIMENTAL is needed for matrix decomposition
target_compile_definitions(ZenithCore PRIVATE GLM_ENABLE_EXPERIMENTAL)

add_subdirectory("ImGui")

target_link_libraries(ZenithCore PUBLIC glad)
target_link_libraries(ZenithCore PUBLIC glm)
target_link_libraries(ZenithCore PUBLIC ImGui)
target_link_libraries(ZenithCore PUBLIC spdlog $<$<BOOL:${MINGW}>:ws2_32>)
target_link_libraries(ZenithCore PUBLIC stb_image)

add_library(
	Zenith STATIC
	"src/Core/main.cpp"
)

target_compile_features(Zenith PRIVATE cxx_std_23)
target_compile_options(Zenith PRIVATE ${ZTH_COMPILE_WARNINGS})
set_property(TARGET Zenith PROPERTY COMPILE_WARNING_AS_ERROR On)

target_link_libraries(Zenith PUBLIC ZenithCore)
