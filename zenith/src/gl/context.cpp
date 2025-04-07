#include "zenith/gl/context.hpp"

#include <glad/glad.h>

#include "zenith/core/assert.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/util/macros.hpp"

namespace zth {

namespace gl {

namespace {

auto debug_callback([[maybe_unused]] GLenum source, GLenum type, [[maybe_unused]] GLuint id, GLenum severity,
                    [[maybe_unused]] GLsizei length, const GLchar* message, [[maybe_unused]] const void* userParam)
    -> void
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
        ZTH_INTERNAL_INFO("[OpenGL] {}: {}", type_str, message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
    case GL_DEBUG_SEVERITY_MEDIUM:
        ZTH_INTERNAL_WARN("[OpenGL] {}: {}", type_str, message);
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        ZTH_INTERNAL_ERROR("[OpenGL] {}: {}", type_str, message);
        ZTH_DEBUG_BREAK;
        break;
    default:
        break;
    }
}

} // namespace

auto set_debug_context() -> void
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debug_callback, nullptr);

    constexpr std::array<GLuint, 1> disabled_messages_api_other = {
        131185, // buffer detailed info from NVIDIA
    };

    glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE,
                          static_cast<GLsizei>(disabled_messages_api_other.size()), disabled_messages_api_other.data(),
                          GL_FALSE);
}

auto log_context_info() -> void
{
    auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    auto version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    auto glsl_version = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    ZTH_INTERNAL_INFO("[OpenGL] Context info:\n"
                      "\tVendor: {}\n"
                      "\tRenderer: {}\n"
                      "\tVersion: {}\n"
                      "\tGLSL Version: {}",
                      vendor, renderer, version, glsl_version);
}

} // namespace gl

auto to_string(gl::Profile gl_profile) -> const char*
{
    switch (gl_profile)
    {
        using enum gl::Profile;
    case Compatibility:
        return "Compatibility";
    case Core:
        return "Core";
    }

    ZTH_ASSERT(false);
    return "Unknown";
}

} // namespace zth

ZTH_DEFINE_FORMATTER(zth::gl::Version, version)
{
    return ZTH_FORMAT_OUT("{}.{}", version.major, version.minor);
}

ZTH_DEFINE_FORMATTER(zth::gl::Profile, profile)
{
    return ZTH_FORMAT_OUT("{}", zth::to_string(profile));
}
