#pragma once

#include <glad/glad.h>

#include "Zenith/Core/Typedefs.hpp"

namespace zth {

enum class AccessFrequency : u8
{
    Stream = 0,  // The data store contents will be modified once and used at most a few times.
    Static = 4,  // The data store contents will be modified once and used many times.
    Dynamic = 8, // The data store contents will be modified repeatedly and used many times.
};

enum class AccessType : u8
{
    Draw = 0, // The data store contents are modified by the application, and used as the source for GL drawing and
              // image specification commands.
    Read = 1, // The data store contents are modified by reading data from the GL, and used to return that data when
              // queried by the application.
    Copy = 2, // The data store contents are modified by reading data from the GL, and used as the source for GL drawing
              // and image specification commands.
};

struct BufferUsage
{
    AccessFrequency access_frequency;
    AccessType access_type;

    static const BufferUsage stream_draw;
    static const BufferUsage static_draw;
    static const BufferUsage dynamic_draw;

    static const BufferUsage stream_read;
    static const BufferUsage static_read;
    static const BufferUsage dynamic_read;

    static const BufferUsage stream_copy;
    static const BufferUsage static_copy;
    static const BufferUsage dynamic_copy;
};

inline const BufferUsage BufferUsage::stream_draw{ AccessFrequency::Stream, AccessType::Draw };
inline const BufferUsage BufferUsage::static_draw{ AccessFrequency::Static, AccessType::Draw };
inline const BufferUsage BufferUsage::dynamic_draw{ AccessFrequency::Dynamic, AccessType::Draw };

inline const BufferUsage BufferUsage::stream_read{ AccessFrequency::Stream, AccessType::Read };
inline const BufferUsage BufferUsage::static_read{ AccessFrequency::Static, AccessType::Read };
inline const BufferUsage BufferUsage::dynamic_read{ AccessFrequency::Dynamic, AccessType::Read };

inline const BufferUsage BufferUsage::stream_copy{ AccessFrequency::Stream, AccessType::Copy };
inline const BufferUsage BufferUsage::static_copy{ AccessFrequency::Static, AccessType::Copy };
inline const BufferUsage BufferUsage::dynamic_copy{ AccessFrequency::Dynamic, AccessType::Copy };

[[nodiscard]] constexpr auto to_gl_enum(BufferUsage buffer_usage) -> GLenum
{
    GLenum res = GL_STREAM_DRAW;
    res += std::to_underlying(buffer_usage.access_frequency);
    res += std::to_underlying(buffer_usage.access_type);
    return res;
}

} // namespace zth
