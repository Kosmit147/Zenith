#pragma once

#include <glad/glad.h>

#include "zenith/core/typedefs.hpp"

namespace zth {

enum class GlBufferAccessFrequency : u8
{
    Stream = 0,  // The data store contents will be modified once and used at most a few times.
    Static = 4,  // The data store contents will be modified once and used many times.
    Dynamic = 8, // The data store contents will be modified repeatedly and used many times.
};

enum class GlBufferAccessType : u8
{
    Draw = 0, // The data store contents are modified by the application, and used as the source for GL drawing and
              // image specification commands.
    Read = 1, // The data store contents are modified by reading data from the GL, and used to return that data when
              // queried by the application.
    Copy = 2, // The data store contents are modified by reading data from the GL, and used as the source for GL drawing
              // and image specification commands.
};

struct GlBufferUsage
{
    GlBufferAccessFrequency access_frequency;
    GlBufferAccessType access_type;

    static const GlBufferUsage stream_draw;
    static const GlBufferUsage static_draw;
    static const GlBufferUsage dynamic_draw;

    static const GlBufferUsage stream_read;
    static const GlBufferUsage static_read;
    static const GlBufferUsage dynamic_read;

    static const GlBufferUsage stream_copy;
    static const GlBufferUsage static_copy;
    static const GlBufferUsage dynamic_copy;
};

inline const GlBufferUsage GlBufferUsage::stream_draw{ GlBufferAccessFrequency::Stream, GlBufferAccessType::Draw };
inline const GlBufferUsage GlBufferUsage::static_draw{ GlBufferAccessFrequency::Static, GlBufferAccessType::Draw };
inline const GlBufferUsage GlBufferUsage::dynamic_draw{ GlBufferAccessFrequency::Dynamic, GlBufferAccessType::Draw };

inline const GlBufferUsage GlBufferUsage::stream_read{ GlBufferAccessFrequency::Stream, GlBufferAccessType::Read };
inline const GlBufferUsage GlBufferUsage::static_read{ GlBufferAccessFrequency::Static, GlBufferAccessType::Read };
inline const GlBufferUsage GlBufferUsage::dynamic_read{ GlBufferAccessFrequency::Dynamic, GlBufferAccessType::Read };

inline const GlBufferUsage GlBufferUsage::stream_copy{ GlBufferAccessFrequency::Stream, GlBufferAccessType::Copy };
inline const GlBufferUsage GlBufferUsage::static_copy{ GlBufferAccessFrequency::Static, GlBufferAccessType::Copy };
inline const GlBufferUsage GlBufferUsage::dynamic_copy{ GlBufferAccessFrequency::Dynamic, GlBufferAccessType::Copy };

[[nodiscard]] constexpr auto to_gl_enum(GlBufferUsage buffer_usage) -> GLenum
{
    GLenum res = GL_STREAM_DRAW;
    res += std::to_underlying(buffer_usage.access_frequency);
    res += std::to_underlying(buffer_usage.access_type);
    return res;
}

} // namespace zth
