#include "zenith/graphics/draw_command.hpp"

namespace zth {

auto DrawCommand::operator==(const DrawCommand& other) const -> bool
{
    // ignore transform
    return vertex_array == other.vertex_array && material == other.material;
}

auto DrawCommand::operator<(const DrawCommand& other) const -> bool
{
    // sort by vertex array first
    if (vertex_array < other.vertex_array)
        return true;
    else if (vertex_array > other.vertex_array)
        return false;

    // sort by material second
    if (material < other.material)
        return true;

    return false;
}

auto DrawCommand::operator>(const DrawCommand& other) const -> bool
{
    // sort by vertex array first
    if (vertex_array > other.vertex_array)
        return true;
    else if (vertex_array < other.vertex_array)
        return false;

    // sort by material second
    if (material > other.material)
        return true;

    return false;
}

auto DrawCommand::operator<=(const DrawCommand& other) const -> bool
{
    return *this < other || *this == other;
}

auto DrawCommand::operator>=(const DrawCommand& other) const -> bool
{
    return *this > other || *this == other;
}

} // namespace zth
