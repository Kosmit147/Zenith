cmake_minimum_required(VERSION 3.28)
project(imgui LANGUAGES CXX)

add_library(imgui STATIC
    "../../dependencies/imgui/imgui.cpp"
	"../../dependencies/imgui/imgui_demo.cpp"
	"../../dependencies/imgui/imgui_draw.cpp"
	"../../dependencies/imgui/imgui_tables.cpp"
	"../../dependencies/imgui/imgui_widgets.cpp"
	"../../dependencies/imgui/backends/imgui_impl_glfw.cpp"
	"../../dependencies/imgui/backends/imgui_impl_opengl3.cpp"
	"../../dependencies/imgui/misc/cpp/imgui_stdlib.cpp"

	"../../dependencies/ImGuizmo/ImGuizmo.cpp"
)

target_include_directories(imgui SYSTEM PUBLIC "../../dependencies/imgui")
target_include_directories(imgui SYSTEM PUBLIC "../../dependencies/imgui/backends")
target_include_directories(imgui SYSTEM PUBLIC "../../dependencies/imgui/misc/cpp")
target_include_directories(imgui SYSTEM PUBLIC "../../dependencies/ImGuizmo")

target_compile_features(imgui PRIVATE cxx_std_23)

target_link_libraries(imgui PUBLIC glfw)
