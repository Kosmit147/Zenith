cmake_minimum_required(VERSION 3.28)
project(ImGui LANGUAGES CXX)

add_library(ImGui STATIC
    "../../dependencies/imgui/imgui.cpp"
	"../../dependencies/imgui/imgui_demo.cpp"
	"../../dependencies/imgui/imgui_draw.cpp"
	"../../dependencies/imgui/imgui_tables.cpp"
	"../../dependencies/imgui/imgui_widgets.cpp"
	"../../dependencies/imgui/backends/imgui_impl_glfw.cpp"
	"../../dependencies/imgui/backends/imgui_impl_opengl3.cpp"

	"../../dependencies/ImGuizmo/ImGuizmo.cpp"
)

target_include_directories(ImGui SYSTEM PUBLIC "../../dependencies/imgui")
target_include_directories(ImGui SYSTEM PUBLIC "../../dependencies/imgui/backends")
target_include_directories(ImGui SYSTEM PUBLIC "../../dependencies/ImGuizmo")

target_compile_features(ImGui PRIVATE cxx_std_23)

target_link_libraries(ImGui PUBLIC glfw)
