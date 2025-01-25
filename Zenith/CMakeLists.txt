cmake_minimum_required(VERSION 3.28)
project(Zenith VERSION 0.1.0 LANGUAGES CXX)

add_library(
	ZenithCore STATIC
	"src/core/application.cpp"
	"src/core/asset_manager.cpp"
	"src/core/camera_controller.cpp"
	"src/core/exception.cpp"
	"src/core/scene_manager.cpp"
	"src/core/system_manager.cpp"
	"src/core/transformable.cpp"
	"src/debug/ui/ui.cpp"
	"src/filesystem/file.cpp"
	"src/graphics/shapes/cube_shape.cpp"
	"src/graphics/shapes/pyramid_shape.cpp"
	"src/graphics/shapes/shape.cpp"
	"src/graphics/shapes/sphere_shape.cpp"
	"src/graphics/camera.cpp"
	"src/graphics/draw_command.cpp"
	"src/graphics/imgui_renderer.cpp"
	"src/graphics/materials.cpp"
	"src/graphics/mesh.cpp"
	"src/graphics/meshes.cpp"
	"src/graphics/renderer.cpp"
	"src/graphics/shaders.cpp"
	"src/logging/logger.cpp"
	"src/math/matrix.cpp"
	"src/math/quaternion.cpp"
	"src/platform/gl/gl_buffer.cpp"
	"src/platform/gl/gl_context.cpp"
	"src/platform/gl/gl_debug.cpp"
	"src/platform/gl/shader.cpp"
	"src/platform/gl/texture.cpp"
	"src/platform/gl/vertex_array.cpp"
	"src/platform/event.cpp"
	"src/platform/event_queue.cpp"
	"src/platform/input.cpp"
	"src/platform/key.cpp"
	"src/platform/mouse.cpp"
	"src/platform/shader_preprocessor.cpp"
	"src/platform/window.cpp"
	"src/time/time.cpp"
	"src/utility/string.cpp"
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
