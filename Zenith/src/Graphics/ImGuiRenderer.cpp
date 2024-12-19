#include "Zenith/Graphics/ImGuiRenderer.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Zenith/Logging/Logger.hpp"
#include "Zenith/Platform/Window.hpp"

namespace zth {

auto ImGuiRenderer::init() -> void
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(Window::glfw_handle(), true);
    ImGui_ImplOpenGL3_Init();

    ZTH_CORE_INFO("ImGui Renderer initialized.");
}

auto ImGuiRenderer::on_update() -> void
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
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

    ZTH_CORE_INFO("ImGui Renderer shut down.");
}

} // namespace zth
