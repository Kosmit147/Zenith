#pragma once

#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <filesystem>

#include "zenith/core/typedefs.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/stl/map.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/stl/vector.hpp"
#include "zenith/util/macros.hpp"
#include "zenith/util/meta.hpp"
#include "zenith/util/optional.hpp"

namespace zth::gl {

enum class ShaderType : u16
{
    Compute = GL_COMPUTE_SHADER,                // 37 305
    Vertex = GL_VERTEX_SHADER,                  // 35 633
    TessControl = GL_TESS_CONTROL_SHADER,       // 36 488
    TessEvaluation = GL_TESS_EVALUATION_SHADER, // 36 487
    Geometry = GL_GEOMETRY_SHADER,              // 36 313
    Fragment = GL_FRAGMENT_SHADER,              // 35 632

    MinEnumValue = Fragment,
    MaxEnumValue = Compute,
};

struct ShaderSources
{
    StringView vertex_source;
    StringView fragment_source;
    Optional<StringView> tess_control_source = nil;
    Optional<StringView> tess_evaluation_source = nil;
    Optional<StringView> geometry_source = nil;
};

struct ShaderSourcePaths
{
    std::filesystem::path vertex_path;
    std::filesystem::path fragment_path;
    Optional<std::filesystem::path> tess_control_path = nil;
    Optional<std::filesystem::path> tess_evaluation_path = nil;
    Optional<std::filesystem::path> geometry_path = nil;
};

struct UniformInfo
{
    GLint location;
    GLsizei size;
};

class Shader
{
public:
    using ShaderId = GLuint;
    using ProgramId = GLuint;

    explicit Shader(const ShaderSources& sources);
    explicit Shader(const ShaderSourcePaths& paths);

    [[nodiscard]] static auto from_sources(const ShaderSources& sources) -> Shader;
    [[nodiscard]] static auto from_files(const ShaderSourcePaths& paths) -> Shader;

    ZTH_NO_COPY(Shader)

    Shader(Shader&& other) noexcept;
    auto operator=(Shader&& other) noexcept -> Shader&;

    ~Shader();

    auto bind() const -> void { glUseProgram(_id); }
    static auto unbind() -> void { glUseProgram(GL_NONE); }

    auto set_unif(StringView name, const auto& val) const -> void;

    [[nodiscard]] auto native_handle() const { return _id; }

private:
    ProgramId _id = GL_NONE;
    UnorderedMap<String, UniformInfo> _uniform_map;

private:
    auto retrieve_unif_info() -> void;
    [[nodiscard]] auto get_unif_info(StringView name) const -> Optional<UniformInfo>;
    [[nodiscard]] auto get_unif_location(StringView name) const -> Optional<GLint>;

    static auto set_unif(GLint location, GLint val) -> void;
    static auto set_unif(GLint location, GLfloat val) -> void;
    static auto set_unif(GLint location, glm::vec2 val) -> void;
    static auto set_unif(GLint location, glm::vec3 val) -> void;
    static auto set_unif(GLint location, glm::vec4 val) -> void;
    static auto set_unif(GLint location, const glm::mat4& val) -> void;

    [[nodiscard]] static auto create_shaders_from_sources(const ShaderSources& sources)
        -> Optional<InPlaceVector<ShaderId, 5>>;
    [[nodiscard]] static auto create_shader(StringView source, ShaderType type) -> Optional<ShaderId>;
    [[nodiscard]] static auto compile_shader(ShaderId id, ShaderType type) -> bool;

    [[nodiscard]] static auto create_program_from_sources(const ShaderSources& sources) -> Optional<ProgramId>;
    [[nodiscard]] static auto create_program_from_files(const ShaderSourcePaths& paths) -> Optional<ProgramId>;
    [[nodiscard]] static auto create_program(const InPlaceVector<ShaderId, 5>& shaders) -> Optional<ProgramId>;
    [[nodiscard]] static auto link_program(ProgramId id) -> bool;

    static auto delete_shaders(const InPlaceVector<ShaderId, 5>& shaders) -> void;

    [[nodiscard]] static auto create_fallback_program() -> Optional<ProgramId>;
};

auto Shader::set_unif(StringView name, const auto& val) const -> void
{
    if (auto location = get_unif_location(name))
        set_unif(location, val);
    else [[unlikely]]
        ZTH_INTERNAL_DEBUG("Uniform with name {} not present in shader {}.", name, _id);
}

[[nodiscard]] auto to_gl_enum(ShaderType shader_type) -> GLenum;

} // namespace zth::gl

ZTH_DECLARE_REFLECTED_ENUM(zth::gl::ShaderType);
