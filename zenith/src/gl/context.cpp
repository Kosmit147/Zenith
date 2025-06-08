#include "zenith/gl/context.hpp"

#include <glad/glad.h>

#include "zenith/log/logger.hpp"
#include "zenith/util/macros.hpp"

namespace zth::gl {

namespace {

auto debug_callback([[maybe_unused]] GLenum source, GLenum type, [[maybe_unused]] GLuint id, GLenum severity,
                    [[maybe_unused]] GLsizei length, const GLchar* message, [[maybe_unused]] const void* userParam)
    -> void
{
    auto type_string = [=] {
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
        ZTH_INTERNAL_INFO("[OpenGL] {}: {}", type_string, message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
    case GL_DEBUG_SEVERITY_MEDIUM:
        ZTH_INTERNAL_WARN("[OpenGL] {}: {}", type_string, message);
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        ZTH_INTERNAL_ERROR("[OpenGL] {}: {}", type_string, message);
        ZTH_DEBUG_BREAK;
        break;
    default:
        break;
    }
}

} // namespace

auto Context::init() -> Result<void, String>
{
    ZTH_INTERNAL_TRACE("Initializing OpenGL context...");

    retrieve_context_variables();

#if defined(ZTH_GL_DEBUG)
    set_debug_context();
#endif

    log_context_info();

    ZTH_INTERNAL_TRACE("OpenGL context initialized.");
    return {};
}

auto Context::shut_down() -> void
{
    ZTH_INTERNAL_TRACE("OpenGL context shut down.");
}

auto Context::set_debug_context() -> void
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debug_callback, nullptr);

    static constexpr std::array<GLuint, 1> disabled_messages_api_other = {
        131185, // Buffer detailed info from NVIDIA.
    };

    glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE,
                          static_cast<GLsizei>(disabled_messages_api_other.size()), disabled_messages_api_other.data(),
                          GL_FALSE);

    _debug_context = true;
}

auto Context::retrieve_context_variables() -> void
{
    {
        _vendor_string = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        _renderer_string = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        _version_string = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        _glsl_version_string = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
    }

    {
        GLint major, minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        _version = Version{ .major = static_cast<u32>(major), .minor = static_cast<u32>(minor) };
    }

    {
        GLint flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        _forward_compatible_context = flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT;
        _debug_context = flags & GL_CONTEXT_FLAG_DEBUG_BIT;
    }
}

auto Context::log_context_info() -> void
{
    ZTH_INTERNAL_INFO("[OpenGL] Context info:\n"
                      "\tVendor: {}\n"
                      "\tRenderer: {}\n"
                      "\tVersion: {}\n"
                      "\tGLSL Version: {}",
                      _vendor_string, _renderer_string, _version_string, _glsl_version_string);
}

} // namespace zth::gl

ZTH_DEFINE_REFLECTED_ENUM(zth::gl::Profile);

ZTH_DEFINE_FORMATTER(zth::gl::Version, version)
{
    return ZTH_FORMAT_OUT("{}.{}", version.major, version.minor);
}
