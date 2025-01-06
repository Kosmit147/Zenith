#pragma once

#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <spdlog/spdlog.h>

#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "Zenith/Core/Assert.hpp"
#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Logging/Logger.hpp"
#include "Zenith/Utility/Macros.hpp"
#include "Zenith/Utility/StringMap.hpp"

namespace zth {

enum class ShaderType : u16
{
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
};

struct UniformInfo
{
    GLint location;
    GLsizei size;
};

class Shader
{
public:
    explicit Shader(std::string_view vertex_source, std::string_view fragment_source);
    ZTH_NO_COPY_NO_MOVE(Shader)
    ~Shader();

    auto bind() const -> void { glUseProgram(_id); }
    static auto unbind() -> void { glUseProgram(GL_NONE); }

    template<typename T> auto set_unif(std::string_view name, const T& val) const -> void
    {
        if (auto info = get_unif_info(name))
            set_unif(info->location, val);
        else [[unlikely]]
            ZTH_CORE_WARN("Uniform with name {} not present in shader with id {}.", name, _id);
    }

    [[nodiscard]] auto native_handle() const { return _id; }

private:
    GLuint _id = GL_NONE;
    StringHashMap<UniformInfo> _uniform_map;

private:
    auto retrieve_unif_info() -> void;
    [[nodiscard]] auto get_unif_info(std::string_view name) const -> std::optional<UniformInfo>;

    static auto set_unif(GLint location, GLint val) -> void;
    static auto set_unif(GLint location, GLfloat val) -> void;
    static auto set_unif(GLint location, glm::vec2 val) -> void;
    static auto set_unif(GLint location, glm::vec3 val) -> void;
    static auto set_unif(GLint location, glm::vec4 val) -> void;
    static auto set_unif(GLint location, const glm::mat4& val) -> void;
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

template<> struct fmt::formatter<zth::ShaderType> : formatter<std::string>
{
    static auto format(zth::ShaderType shader_type, format_context& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "{}", zth::to_string(shader_type));
    }
};
