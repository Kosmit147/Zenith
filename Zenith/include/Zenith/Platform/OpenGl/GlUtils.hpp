#pragma once

#include <glad/glad.h>

namespace zth {

template<typename T> [[nodiscard]] constexpr auto to_gl_enum() -> GLenum
{
    static_assert(false, "not implemented");
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

} // namespace zth
