#pragma once

#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <array>
#include <filesystem>

#include "zenith/core/typedefs.hpp"
#include "zenith/log/format.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/stl/string_map.hpp"
#include "zenith/stl/vector.hpp"
#include "zenith/util/macros.hpp"
#include "zenith/util/optional.hpp"

namespace zth {

namespace gl {

enum class ShaderType : u16
{
    Compute = GL_COMPUTE_SHADER,
    Vertex = GL_VERTEX_SHADER,
    TessControl = GL_TESS_CONTROL_SHADER,
    TessEvaluation = GL_TESS_EVALUATION_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
};

constexpr std::array shader_type_enumerations = {
    ShaderType::Compute,        ShaderType::Vertex,   ShaderType::TessControl,
    ShaderType::TessEvaluation, ShaderType::Geometry, ShaderType::Fragment,
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

    template<typename T> auto set_unif(StringView name, const T& val) const -> void
    {
        if (auto info = get_unif_info(name))
            set_unif(info->location, val);
        else [[unlikely]]
            ZTH_CORE_DEBUG("Uniform with name {} not present in shader {}.", name, _id);
    }

    [[nodiscard]] auto native_handle() const { return _id; }

private:
    ProgramId _id = GL_NONE;
    StringHashMap<UniformInfo> _uniform_map;

private:
    auto retrieve_unif_info() -> void;
    [[nodiscard]] auto get_unif_info(StringView name) const -> Optional<UniformInfo>;

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

[[nodiscard]] auto to_gl_enum(ShaderType shader_type) -> GLenum;

} // namespace gl

[[nodiscard]] auto to_string(gl::ShaderType shader_type) -> const char*;

} // namespace zth

ZTH_DECLARE_FORMATTER(zth::gl::ShaderType);
ZTH_DECLARE_FORMATTER(zth::gl::ShaderSourcePaths);
