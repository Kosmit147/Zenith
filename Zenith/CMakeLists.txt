cmake_minimum_required(VERSION 3.28)
project(Zenith VERSION 0.0.1 LANGUAGES CXX)

set(imgui_sources
    "../dependencies/imgui/imgui.cpp"
	"../dependencies/imgui/imgui_demo.cpp"
	"../dependencies/imgui/imgui_draw.cpp"
	"../dependencies/imgui/imgui_tables.cpp"
	"../dependencies/imgui/imgui_widgets.cpp"
	"../dependencies/imgui/backends/imgui_impl_glfw.cpp"
	"../dependencies/imgui/backends/imgui_impl_opengl3.cpp"
)

add_library(
	ZenithCore STATIC
	"src/Core/Application.cpp"
	"src/Core/CameraController.cpp"
	"src/Core/Exception.cpp"
	"src/Core/SceneManager.cpp"
	"src/Core/SystemManager.cpp"
	"src/Core/Transformable.cpp"
	"src/Graphics/Shapes/CubeShape.cpp"
	"src/Graphics/Shapes/Shape.cpp"
	"src/Graphics/Shapes/SphereShape.cpp"
	"src/Graphics/Camera.cpp"
	"src/Graphics/DrawCommand.cpp"
	"src/Graphics/ImGuiRenderer.cpp"
	"src/Graphics/Light.cpp"
	"src/Graphics/Materials.cpp"
	"src/Graphics/Mesh.cpp"
	"src/Graphics/Meshes.cpp"
	"src/Graphics/Renderer.cpp"
	"src/Graphics/Shaders.cpp"
	"src/Logging/Logger.cpp"
	"src/Math/Matrix.cpp"
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

	${imgui_sources}
)

b_embed(ZenithCore "include/Zenith/Graphics/ShaderDefines.h")

b_embed(ZenithCore "src/Graphics/Shaders/flat_color.vert")
b_embed(ZenithCore "src/Graphics/Shaders/flat_color.frag")
b_embed(ZenithCore "src/Graphics/Shaders/standard.vert")
b_embed(ZenithCore "src/Graphics/Shaders/standard.frag")

target_include_directories(ZenithCore SYSTEM PUBLIC "../dependencies/imgui")
target_include_directories(ZenithCore SYSTEM PUBLIC "../dependencies/imgui/backends")
target_include_directories(ZenithCore PUBLIC "include")
target_compile_features(ZenithCore PRIVATE cxx_std_23)
target_compile_options(ZenithCore PRIVATE ${ZTH_COMPILE_WARNINGS})
target_precompile_headers(ZenithCore PRIVATE "src/pch.hpp")
set_property(TARGET ZenithCore PROPERTY COMPILE_WARNING_AS_ERROR On)

# GLM_ENABLE_EXPERIMENTAL is needed for matrix decomposition
# _CRT_SECURE_NO_WARNINGS is needed for imgui implementation files
target_compile_definitions(ZenithCore PRIVATE GLM_ENABLE_EXPERIMENTAL _CRT_SECURE_NO_WARNINGS)

target_link_libraries(ZenithCore PUBLIC glad)
target_link_libraries(ZenithCore PUBLIC glfw)
target_link_libraries(ZenithCore PUBLIC spdlog $<$<BOOL:${MINGW}>:ws2_32>)
target_link_libraries(ZenithCore PUBLIC glm)
target_link_libraries(ZenithCore PUBLIC stb_image)

add_library(
	Zenith STATIC
	"src/Core/main.cpp"
)

target_compile_features(Zenith PRIVATE cxx_std_23)
target_compile_options(Zenith PRIVATE ${ZTH_COMPILE_WARNINGS})
set_property(TARGET Zenith PROPERTY COMPILE_WARNING_AS_ERROR On)

target_link_libraries(Zenith PUBLIC ZenithCore)
