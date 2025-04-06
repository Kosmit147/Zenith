#pragma once

#include <glad/glad.h>
#include <glm/fwd.hpp>

#include "zenith/core/assert.hpp"
#include "zenith/core/typedefs.hpp"

namespace zth::gl {

using GlslBool = GLuint;

enum class DataType : u16
{
    None = GL_NONE,
    UnsignedByte = GL_UNSIGNED_BYTE,
    Byte = GL_BYTE,
    UnsignedShort = GL_UNSIGNED_SHORT,
    Short = GL_SHORT,
    UnsignedInt = GL_UNSIGNED_INT,
    Int = GL_INT,
    Float = GL_FLOAT,
    Double = GL_DOUBLE,
};

template<typename T> [[nodiscard]] constexpr auto to_data_type() -> DataType
{
    static_assert(false, "not implemented");
    return DataType::None;
}

template<> [[nodiscard]] constexpr auto to_data_type<GLubyte>() -> DataType
{
    return DataType::UnsignedByte;
}

template<> [[nodiscard]] constexpr auto to_data_type<GLbyte>() -> DataType
{
    return DataType::Byte;
}

template<> [[nodiscard]] constexpr auto to_data_type<GLushort>() -> DataType
{
    return DataType::UnsignedShort;
}

template<> [[nodiscard]] constexpr auto to_data_type<GLshort>() -> DataType
{
    return DataType::Short;
}

template<> [[nodiscard]] constexpr auto to_data_type<GLuint>() -> DataType
{
    return DataType::UnsignedInt;
}

template<> [[nodiscard]] constexpr auto to_data_type<GLint>() -> DataType
{
    return DataType::Int;
}

template<> [[nodiscard]] constexpr auto to_data_type<GLfloat>() -> DataType
{
    return DataType::Float;
}

template<> [[nodiscard]] constexpr auto to_data_type<GLdouble>() -> DataType
{
    return DataType::Double;
}

[[nodiscard]] constexpr auto to_gl_enum(DataType type) -> GLenum
{
    switch (type)
    {
        using enum DataType;
    case None:
        return GL_NONE;
    case UnsignedByte:
        return GL_UNSIGNED_BYTE;
    case Byte:
        return GL_BYTE;
    case UnsignedShort:
        return GL_UNSIGNED_SHORT;
    case Short:
        return GL_SHORT;
    case UnsignedInt:
        return GL_UNSIGNED_INT;
    case Int:
        return GL_INT;
    case Float:
        return GL_FLOAT;
    case Double:
        return GL_DOUBLE;
    }

    ZTH_ASSERT(false);
    return GL_NONE;
}

[[nodiscard]] constexpr auto is_an_index_data_type(DataType type) -> bool
{
    using enum DataType;
    return type == UnsignedByte || type == UnsignedShort || type == UnsignedInt;
}

[[nodiscard]] constexpr auto size_of_data_type(DataType type) -> usize
{
    switch (type)
    {
        using enum DataType;
    case None:
        return 0;
    case UnsignedByte:
        return sizeof(GLubyte);
    case Byte:
        return sizeof(GLbyte);
    case UnsignedShort:
        return sizeof(GLushort);
    case Short:
        return sizeof(GLshort);
    case UnsignedInt:
        return sizeof(GLuint);
    case Int:
        return sizeof(GLint);
    case Float:
        return sizeof(GLfloat);
    case Double:
        return sizeof(GLdouble);
    }

    ZTH_ASSERT(false);
    return 0;
}

template<typename T> [[nodiscard]] constexpr auto get_std140_field_alignment() -> usize
{
    // In the default case we assume T is a struct.
    return 16;
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
    return alignof(T);
}

template<> [[nodiscard]] constexpr auto get_std430_field_alignment<GLushort>() -> usize
{
    return 2;
}

template<> [[nodiscard]] constexpr auto get_std430_field_alignment<GLshort>() -> usize
{
    return 2;
}

template<> [[nodiscard]] constexpr auto get_std430_field_alignment<GLuint>() -> usize
{
    return 4;
}

template<> [[nodiscard]] constexpr auto get_std430_field_alignment<GLint>() -> usize
{
    return 4;
}

template<> [[nodiscard]] constexpr auto get_std430_field_alignment<GLfloat>() -> usize
{
    return 4;
}

template<> [[nodiscard]] constexpr auto get_std430_field_alignment<GLdouble>() -> usize
{
    return 8;
}

template<> [[nodiscard]] constexpr auto get_std430_field_alignment<glm::vec2>() -> usize
{
    return 8;
}

template<> [[nodiscard]] constexpr auto get_std430_field_alignment<glm::vec3>() -> usize
{
    return 16;
}

template<> [[nodiscard]] constexpr auto get_std430_field_alignment<glm::vec4>() -> usize
{
    return 16;
}

template<> [[nodiscard]] constexpr auto get_std430_field_alignment<glm::mat4>() -> usize
{
    return 16;
}

} // namespace zth::gl
