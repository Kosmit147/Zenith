cmake_minimum_required(VERSION 3.28)
project(zenith VERSION 0.1.0 LANGUAGES CXX)

add_library(
	zenith_core STATIC
	"src/asset/asset.cpp"
	"src/core/exception.cpp"
	"src/core/random.cpp"
	"src/core/scene.cpp"
	"src/debug/ui.cpp"
	"src/ecs/components.cpp"
	"src/ecs/ecs.cpp"
	"src/embedded/shaders.cpp"
	"src/gl/buffer.cpp"
	"src/gl/context.cpp"
	"src/gl/shader.cpp"
	"src/gl/texture.cpp"
	"src/gl/vertex_array.cpp"
	"src/gl/vertex_layout.cpp"
	"src/layer/layer.cpp"
	"src/layer/layers.cpp"
	"src/log/formatters.cpp"
	"src/log/logger.cpp"
	"src/math/matrix.cpp"
	"src/math/quaternion.cpp"
	"src/memory/alloc.cpp"
	"src/memory/buffer.cpp"
	"src/memory/temporary_storage.cpp"
	"src/renderer/resources/buffers.cpp"
	"src/renderer/resources/materials.cpp"
	"src/renderer/resources/meshes.cpp"
	"src/renderer/resources/shaders.cpp"
	"src/renderer/resources/textures.cpp"
	"src/renderer/imgui_renderer.cpp"
	"src/renderer/light.cpp"
	"src/renderer/mesh.cpp"
	"src/renderer/renderer.cpp"
	"src/renderer/shader_preprocessor.cpp"
	"src/script/camera_controller.cpp"
	"src/stl/string_algorithm.cpp"
	"src/system/application.cpp"
	"src/system/event.cpp"
	"src/system/event_queue.cpp"
	"src/system/file.cpp"
	"src/system/input.cpp"
	"src/system/time.cpp"
	"src/system/window.cpp"
)

b_embed(zenith_core "src/shaders/zth_defines.glsl")
b_embed(zenith_core "src/shaders/zth_fallback.vert")
b_embed(zenith_core "src/shaders/zth_fallback.frag")
b_embed(zenith_core "src/shaders/zth_flat_color.vert")
b_embed(zenith_core "src/shaders/zth_flat_color.frag")
b_embed(zenith_core "src/shaders/zth_standard.vert")
b_embed(zenith_core "src/shaders/zth_standard.frag")

target_include_directories(zenith_core PUBLIC "include")
target_compile_features(zenith_core PRIVATE cxx_std_23)
target_compile_options(zenith_core PRIVATE ${ZTH_COMPILE_WARNINGS})
target_precompile_headers(zenith_core PRIVATE "src/pch.hpp")
set_property(TARGET zenith_core PROPERTY COMPILE_WARNING_AS_ERROR On)

set(ZTH_PRIVATE_DEFINITIONS
    _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING # For battery::embed.
)

set(ZTH_PUBLIC_DEFINITIONS
    GLM_ENABLE_EXPERIMENTAL # For matrix decomposition.
)

if(ZTH_ASSERTIONS)
    list(APPEND ZTH_PUBLIC_DEFINITIONS "ZTH_ASSERTIONS")
endif()

if(ZTH_GL_DEBUG)
    list(APPEND ZTH_PUBLIC_DEFINITIONS "ZTH_GL_DEBUG")
endif()

if(ZTH_DEBUG_LOGGING)
    list(APPEND ZTH_PUBLIC_DEFINITIONS "ZTH_DEBUG_LOGGING")
endif()

target_compile_definitions(zenith_core PRIVATE ${ZTH_PRIVATE_DEFINITIONS})
target_compile_definitions(zenith_core PUBLIC ${ZTH_PUBLIC_DEFINITIONS})

add_subdirectory("imgui")

target_link_libraries(zenith_core PUBLIC EnTT)
target_link_libraries(zenith_core PUBLIC glad)
target_link_libraries(zenith_core PUBLIC glfw)
target_link_libraries(zenith_core PUBLIC glm)
target_link_libraries(zenith_core PUBLIC imgui)
target_link_libraries(zenith_core PUBLIC spdlog $<$<BOOL:${MINGW}>:ws2_32>)
target_link_libraries(zenith_core PUBLIC stb_image)

add_library(
    zenith STATIC
    "src/core/main.cpp"
)

target_compile_features(zenith PRIVATE cxx_std_23)
target_compile_options(zenith PRIVATE ${ZTH_COMPILE_WARNINGS})
target_precompile_headers(zenith REUSE_FROM zenith_core)
set_property(TARGET zenith PROPERTY COMPILE_WARNING_AS_ERROR On)

target_compile_definitions(zenith PRIVATE ${ZTH_PRIVATE_DEFINITIONS})
target_compile_definitions(zenith PUBLIC ${ZTH_PUBLIC_DEFINITIONS})

target_link_libraries(zenith PUBLIC zenith_core)
