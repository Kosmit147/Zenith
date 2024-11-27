#pragma once

#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "Zenith/Core/Assert.hpp"
#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Logging/Logger.hpp"
#include "Zenith/Utility/Utility.hpp"

namespace zth {

enum class ShaderType : u8
{
    Vertex,
    Fragment
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
    auto unbind() const -> void { glUseProgram(0); }

    template<typename T> auto set_unif(const std::string& name, const T& val) const -> void
    {
        auto info = get_unif_info(name);

        if (info)
            set_unif(info.value().location, val);
        else
            ZTH_CORE_WARN("Uniform with name {} not present in shader with id {}.", name, _id);
    }

    [[nodiscard]] auto native_handle() const { return _id; }

private:
    GLuint _id = GL_NONE;
    std::unordered_map<std::string, UniformInfo> _uniform_map;

private:
    auto retrieve_unif_info() -> void;
    [[nodiscard]] auto get_unif_info(const std::string& name) const -> std::optional<UniformInfo>;

    auto set_unif(GLint location, GLint val) const -> void;
    auto set_unif(GLint location, GLfloat val) const -> void;
    auto set_unif(GLint location, glm::vec2 val) const -> void;
    auto set_unif(GLint location, glm::vec3 val) const -> void;
    auto set_unif(GLint location, glm::vec4 val) const -> void;
    auto set_unif(GLint location, const glm::mat4& val) const -> void;
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
