#include "zenith/graphics/imgui_renderer.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include "zenith/logging/logger.hpp"
#include "zenith/platform/window.hpp"

namespace zth {

auto ImGuiRenderer::init() -> void
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(Window::glfw_handle(), true);
    ImGui_ImplOpenGL3_Init();
    ZTH_CORE_INFO("ImGui renderer initialized.");

    ImGui::GetIO().FontGlobalScale = initial_font_scale;
}

auto ImGuiRenderer::on_update() -> void
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    ImGuizmo::BeginFrame();
    auto& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
}

auto ImGuiRenderer::on_render() -> void
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

auto ImGuiRenderer::shut_down() -> void
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    ZTH_CORE_INFO("ImGui renderer shut down.");
}

} // namespace zth
