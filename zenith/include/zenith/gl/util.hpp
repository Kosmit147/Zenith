#pragma once

#include <glad/glad.h>
#include <glm/fwd.hpp>

#include <limits>

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

template<typename T> constexpr DataType to_data_type; // Must be specialized.

template<> constexpr auto to_data_type<GLubyte> = DataType::UnsignedByte;
template<> constexpr auto to_data_type<GLbyte> = DataType::Byte;
template<> constexpr auto to_data_type<GLushort> = DataType::UnsignedShort;
template<> constexpr auto to_data_type<GLshort> = DataType::Short;
template<> constexpr auto to_data_type<GLuint> = DataType::UnsignedInt;
template<> constexpr auto to_data_type<GLint> = DataType::Int;
template<> constexpr auto to_data_type<GLfloat> = DataType::Float;
template<> constexpr auto to_data_type<GLdouble> = DataType::Double;

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

[[nodiscard]] constexpr auto is_an_indexing_data_type(DataType type) -> bool
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

// Returns the smallest unsigned integral data type that can store the given number.
[[nodiscard]] constexpr auto smallest_indexing_data_type(usize number) -> DataType
{
    if (number <= std::numeric_limits<u8>::max())
    {
        static_assert(size_of_data_type(DataType::UnsignedByte) == sizeof(u8));
        return DataType::UnsignedByte;
    }

    if (number <= std::numeric_limits<u16>::max())
    {
        static_assert(size_of_data_type(DataType::UnsignedShort) == sizeof(u16));
        return DataType::UnsignedShort;
    }

    if (number <= std::numeric_limits<u32>::max())
    {
        static_assert(size_of_data_type(DataType::UnsignedInt) == sizeof(u32));
        return DataType::UnsignedInt;
    }

    ZTH_CONSTEVAL_OR_REGULAR_ASSERT(false);
    return DataType::None;
}

template<typename T> constexpr usize std140_field_alignment = 16; // In the default case we assume T is a struct.

template<> constexpr usize std140_field_alignment<GLushort> = 2;
template<> constexpr usize std140_field_alignment<GLshort> = 2;
template<> constexpr usize std140_field_alignment<GLuint> = 4;
template<> constexpr usize std140_field_alignment<GLint> = 4;
template<> constexpr usize std140_field_alignment<GLfloat> = 4;
template<> constexpr usize std140_field_alignment<GLdouble> = 8;
template<> constexpr usize std140_field_alignment<glm::vec2> = 8;
template<> constexpr usize std140_field_alignment<glm::vec3> = 16;
template<> constexpr usize std140_field_alignment<glm::vec4> = 16;
template<> constexpr usize std140_field_alignment<glm::mat4> = 16;

template<typename T> constexpr usize std430_field_alignment = alignof(T);

template<> constexpr usize std430_field_alignment<GLushort> = 2;
template<> constexpr usize std430_field_alignment<GLshort> = 2;
template<> constexpr usize std430_field_alignment<GLuint> = 4;
template<> constexpr usize std430_field_alignment<GLint> = 4;
template<> constexpr usize std430_field_alignment<GLfloat> = 4;
template<> constexpr usize std430_field_alignment<GLdouble> = 8;
template<> constexpr usize std430_field_alignment<glm::vec2> = 8;
template<> constexpr usize std430_field_alignment<glm::vec3> = 16;
template<> constexpr usize std430_field_alignment<glm::vec4> = 16;
template<> constexpr usize std430_field_alignment<glm::mat4> = 16;

} // namespace zth::gl
