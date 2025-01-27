#include "zenith/gl/buffer_usage.hpp"

namespace zth::gl {

auto to_gl_enum(BufferUsage buffer_usage) -> GLenum
{
    GLenum res = GL_STREAM_DRAW;
    res += std::to_underlying(buffer_usage.access_frequency);
    res += std::to_underlying(buffer_usage.access_type);
    return res;
}

} // namespace zth::gl
