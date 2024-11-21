#pragma once

#include <glad/glad.h>

#include <string_view>
#include <utility>

#include "Zenith/Core/Assert.hpp"
#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Utility/Utility.hpp"

namespace zth {

enum class ShaderType : u8
{
    Vertex,
    Fragment
};

class Shader
{
public:
    explicit Shader(std::string_view vertex_source, std::string_view fragment_source);
    ZTH_NO_COPY_NO_MOVE(Shader)
    ~Shader();

    auto bind() const -> void { glUseProgram(_id); }
    auto unbind() const -> void { glUseProgram(0); }

    [[nodiscard]] auto native_handle() const { return _id; }

private:
    GLuint _id = 0; // 0 is not a valid shader id
};

[[nodiscard]] constexpr auto to_gl_enum(ShaderType shader_type) -> GLenum
{
    switch (shader_type)
    {
        using enum ShaderType;
    case Vertex:
        return GL_VERTEX_SHADER;
    case Fragment:
        return GL_FRAGMENT_SHADER;
    }

    ZTH_ASSERT(false);
    std::unreachable();
}

[[nodiscard]] constexpr auto to_string(ShaderType shader_type) -> const char*
{
    switch (shader_type)
    {
        using enum ShaderType;
    case Vertex:
        return "Vertex";
    case Fragment:
        return "Fragment";
    }

    ZTH_ASSERT(false);
    return "Unknown";
}

} // namespace zth
