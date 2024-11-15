#include "Zenith/Platform/Window.hpp"

namespace zth {

namespace {

bool glfw_initialized = false;
bool glad_initialized = false;

auto ensure_glfw_initialized() -> bool
{
    if (glfw_initialized)
        return true;

    if (!glfwInit())
        return false;

    glfw_initialized = true;
    return true;
}

auto terminate_glfw() -> void
{
    if (!glfw_initialized)
        return;

    glfwTerminate();
    glfw_initialized = false;
}

auto ensure_glad_initialized() -> bool
{
    if (glad_initialized)
        return true;

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        return false;

    glad_initialized = true;
    return true;
}

} // namespace

auto Window::create(const WindowSpec& spec) -> std::optional<Window>
{
    Window window(spec);

    if (window._window == nullptr)
        return {};

    return std::optional{ std::move(window) };
}

Window::Window(Window&& other) noexcept : _window(other._window)
{
    other._window = nullptr;
}

auto Window::operator=(Window&& other) noexcept -> Window&
{
    _window = other._window;
    other._window = nullptr;
    return *this;
}

Window::~Window()
{
    if (!_window)
        return;

    glfwDestroyWindow(_window);
    _window_count--;

    if (_window_count == 0)
        terminate_glfw();
}

auto Window::set_resize_callback(OnResizeCallback callback) const -> void
{
    glfwSetFramebufferSizeCallback(_window, callback);
}

Window::Window(const WindowSpec& spec)
{
    if (!ensure_glfw_initialized())
    {
        std::println(std::cerr, "Failed to initialize GLFW.");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, static_cast<int>(spec.gl_version.major));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, static_cast<int>(spec.gl_version.minor));
    glfwWindowHint(GLFW_OPENGL_PROFILE, static_cast<int>(to_gl_enum(spec.gl_profile)));

    _window = glfwCreateWindow(static_cast<int>(spec.width), static_cast<int>(spec.height), spec.title.data(), nullptr,
                               nullptr);

    if (!_window)
    {
        if (_window_count == 0)
            terminate_glfw();

        std::println(std::cerr, "Failed to create a window.");
        return;
    }

    set_active();
    set_vsync(spec.vsync);

    if (!ensure_glad_initialized())
    {
        if (_window_count == 0)
        {
            glfwDestroyWindow(_window);
            terminate_glfw();
        }

        std::println(std::cerr, "Failed to initialize glad.");
        return;
    }

    glViewport(0, 0, static_cast<GLsizei>(spec.width), static_cast<GLsizei>(spec.height));
    set_resize_callback([]([[maybe_unused]] GLFWwindow* window, int new_width, int new_height) {
        glViewport(0, 0, new_width, new_height);
    });

    _window_count++;
}

} // namespace zth
