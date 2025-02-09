#pragma once

#include <glad/glad.h>
#include <glm/fwd.hpp>

#include "zenith/core/typedefs.hpp"

namespace zth::gl {

using GlslBool = GLuint;

template<typename T> [[nodiscard]] constexpr auto to_gl_enum() -> GLenum
{
    static_assert(false, "not implemented");
    return GL_NONE;
}

template<> [[nodiscard]] constexpr auto to_gl_enum<GLubyte>() -> GLenum
{
    return GL_UNSIGNED_BYTE;
}

template<> [[nodiscard]] constexpr auto to_gl_enum<GLbyte>() -> GLenum
{
    return GL_BYTE;
}

template<> [[nodiscard]] constexpr auto to_gl_enum<GLushort>() -> GLenum
{
    return GL_UNSIGNED_SHORT;
}

template<> [[nodiscard]] constexpr auto to_gl_enum<GLshort>() -> GLenum
{
    return GL_SHORT;
}

template<> [[nodiscard]] constexpr auto to_gl_enum<GLuint>() -> GLenum
{
    return GL_UNSIGNED_INT;
}

template<> [[nodiscard]] constexpr auto to_gl_enum<GLint>() -> GLenum
{
    return GL_INT;
}

template<typename T> [[nodiscard]] constexpr auto get_std140_field_alignment() -> usize
{
    static_assert(false, "not implemented");
}

template<> [[nodiscard]] constexpr auto get_std140_field_alignment<GLushort>() -> usize
{
    return 2;
}

template<> [[nodiscard]] constexpr auto get_std140_field_alignment<GLshort>() -> usize
{
    return 2;
}

template<> [[nodiscard]] constexpr auto get_std140_field_alignment<GLuint>() -> usize
{
    return 4;
}

template<> [[nodiscard]] constexpr auto get_std140_field_alignment<GLint>() -> usize
{
    return 4;
}

template<> [[nodiscard]] constexpr auto get_std140_field_alignment<GLfloat>() -> usize
{
    return 4;
}

template<> [[nodiscard]] constexpr auto get_std140_field_alignment<GLdouble>() -> usize
{
    return 8;
}

template<> [[nodiscard]] constexpr auto get_std140_field_alignment<glm::vec2>() -> usize
{
    return 8;
}

template<> [[nodiscard]] constexpr auto get_std140_field_alignment<glm::vec3>() -> usize
{
    return 16;
}

template<> [[nodiscard]] constexpr auto get_std140_field_alignment<glm::vec4>() -> usize
{
    return 16;
}

template<> [[nodiscard]] constexpr auto get_std140_field_alignment<glm::mat4>() -> usize
{
    return 16;
}

template<typename T> [[nodiscard]] constexpr auto get_std430_field_alignment() -> usize
{
    return get_std140_field_alignment<T>();
}

} // namespace zth::gl
