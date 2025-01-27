#pragma once

#include <glad/glad.h>

#include "zenith/core/typedefs.hpp"

namespace zth::gl {

enum class BufferAccessFrequency : u8
{
    Stream = 0,  // The data store contents will be modified once and used at most a few times.
    Static = 4,  // The data store contents will be modified once and used many times.
    Dynamic = 8, // The data store contents will be modified repeatedly and used many times.
};

enum class BufferAccessType : u8
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
    BufferAccessFrequency access_frequency;
    BufferAccessType access_type;

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

inline const BufferUsage BufferUsage::stream_draw{ BufferAccessFrequency::Stream, BufferAccessType::Draw };
inline const BufferUsage BufferUsage::static_draw{ BufferAccessFrequency::Static, BufferAccessType::Draw };
inline const BufferUsage BufferUsage::dynamic_draw{ BufferAccessFrequency::Dynamic, BufferAccessType::Draw };

inline const BufferUsage BufferUsage::stream_read{ BufferAccessFrequency::Stream, BufferAccessType::Read };
inline const BufferUsage BufferUsage::static_read{ BufferAccessFrequency::Static, BufferAccessType::Read };
inline const BufferUsage BufferUsage::dynamic_read{ BufferAccessFrequency::Dynamic, BufferAccessType::Read };

inline const BufferUsage BufferUsage::stream_copy{ BufferAccessFrequency::Stream, BufferAccessType::Copy };
inline const BufferUsage BufferUsage::static_copy{ BufferAccessFrequency::Static, BufferAccessType::Copy };
inline const BufferUsage BufferUsage::dynamic_copy{ BufferAccessFrequency::Dynamic, BufferAccessType::Copy };

[[nodiscard]] constexpr auto to_gl_enum(BufferUsage buffer_usage) -> GLenum
{
    GLenum res = GL_STREAM_DRAW;
    res += std::to_underlying(buffer_usage.access_frequency);
    res += std::to_underlying(buffer_usage.access_type);
    return res;
}

} // namespace zth::gl
