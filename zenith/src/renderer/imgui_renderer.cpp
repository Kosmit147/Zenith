#include "zenith/renderer/imgui_renderer.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include "zenith/log/logger.hpp"
#include "zenith/system/window.hpp"

namespace zth {

auto ImGuiRenderer::init() -> Result<void, String>
{
    ZTH_INTERNAL_TRACE("Initializing ImGui renderer...");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(Window::glfw_handle(), true);
    ImGui_ImplOpenGL3_Init();

    auto& io = ImGui::GetIO();
    io.FontGlobalScale = initial_font_scale;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ZTH_INTERNAL_TRACE("ImGui renderer initialized.");
    return {};
}

auto ImGuiRenderer::start_frame() -> void
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport(ImGui::GetID("Main Window Dockspace"), ImGui::GetMainViewport(),
                                 ImGuiDockNodeFlags_PassthruCentralNode);

    ImGuizmo::BeginFrame();
    auto& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
}

auto ImGuiRenderer::render() -> void
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();

    // Platform functions may change the current OpenGL context, so we need to restore it.
    Window::make_context_current();
}

auto ImGuiRenderer::shut_down() -> void
{
    ZTH_INTERNAL_TRACE("Shutting down ImGui renderer...");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    ZTH_INTERNAL_TRACE("ImGui renderer shut down.");
}

} // namespace zth
