#include "Zenith/Platform/OpenGl/GlDebug.hpp"

#include <glad/glad.h>

#include "Zenith/Logging/Logger.hpp"
#include "Zenith/Utility/Macros.hpp"

namespace zth {

namespace {

auto gl_debug_callback([[maybe_unused]] GLenum source, GLenum type, [[maybe_unused]] GLuint id, GLenum severity,
                       [[maybe_unused]] GLsizei length, const GLchar* message,
                       [[maybe_unused]] const void* userParam) -> void
{
    auto type_str = [=] {
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:
            return "Error";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return "Deprecated behavior";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return "Undefined behavior";
        case GL_DEBUG_TYPE_PORTABILITY:
            return "Portability";
        case GL_DEBUG_TYPE_PERFORMANCE:
            return "Performance";
        case GL_DEBUG_TYPE_MARKER:
            return "Marker";
        case GL_DEBUG_TYPE_OTHER:
            return "Other";
        default:
            return "";
        }
    }();

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        ZTH_CORE_INFO("[OpenGL] {}: {}", type_str, message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
    case GL_DEBUG_SEVERITY_MEDIUM:
        ZTH_CORE_WARN("[OpenGL] {}: {}", type_str, message);
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        ZTH_CORE_ERROR("[OpenGL] {}: {}", type_str, message);
        ZTH_DEBUG_BREAK;
        break;
    default:
        break;
    }
}

} // namespace

auto enable_gl_debug() -> void
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_debug_callback, nullptr);

    constexpr std::array<GLuint, 1> disabled_messages_api_other = {
        131185, // buffer detailed info from NVIDIA
    };

    glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE,
                          static_cast<GLsizei>(disabled_messages_api_other.size()), disabled_messages_api_other.data(),
                          GL_FALSE);
}

} // namespace zth
